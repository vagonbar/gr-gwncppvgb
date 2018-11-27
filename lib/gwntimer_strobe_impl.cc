/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "gwntimer_strobe_impl.h"

// for timers
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdexcept>
#include <string.h>



namespace gr {
  namespace gwncppvgb {


    gwntimer_strobe_impl::GWNTimer::GWNTimer(
      gwntimer_strobe_impl * p_block, int p_id_timer, 
      pmt::pmt_t p_pmt_msg, int p_count, float p_period_ms)
    {

      d_block = p_block;
      d_id_timer = p_id_timer;
      d_pmt_msg = p_pmt_msg;
      d_count = p_count;
      d_counter = 0;
      d_period_ms = p_period_ms;
      d_suspend = false;  // always emits first message

      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind (
            &gwntimer_strobe_impl::GWNTimer::run_timer, this))
        );
    }  // end GWNTimer::GWNTimer



    void
    gwntimer_strobe_impl::GWNTimer::run_timer()
    {
      boost::mutex d_mutex;

      while( d_counter < d_count) {
        d_counter = d_counter + 1;
        // first sleep for d_period_ms milliseconds
        boost::this_thread::sleep( 
          boost::posix_time::milliseconds(d_period_ms));
        if ( d_suspend == false ) // timer is not suspended
        {
          //d_block->post_timer_msg(d_msg);
          pmt::pmt_t pmt_port = pmt::mp("timer_port");
          pmt::pmt_t pmt_id_timer = pmt::mp(d_id_timer);
          pmt::pmt_t pmt_id_counter = pmt::mp(d_counter);
          pmt::pmt_t pmt_tuple_emit = pmt::make_tuple(pmt_port, 
            pmt_id_timer, pmt_id_counter, d_pmt_msg);
          
          d_mutex.lock();
          d_block->gr::basic_block::_post(pmt_port, pmt_tuple_emit);
          d_mutex.unlock();
        }  // end if
      } // end while

      std::cout << "    === TIMER FINISHED: " << d_id_timer <<
        ", counter: " << d_counter << 
        ", thread id: " << d_thread->get_id() << std::endl;
        d_thread->interrupt();  // end thread
      return;
 
    } // end run_timer()



    gwntimer_strobe::sptr
    gwntimer_strobe::make (
        std::string msg_1, float period_1, int count_1,
        std::string msg_2, float period_2, int count_2 ) 

    {
      return gnuradio::get_initial_sptr
        (new gwntimer_strobe_impl (
          msg_1, period_1, count_1, msg_2, period_2, count_2
        ) );
    }



    /* The private constructor */
    gwntimer_strobe_impl::gwntimer_strobe_impl (
        std::string msg_1, float period_1, int count_1,
        std::string msg_2, float period_2, int count_2) 
      : gr::block("gwntimer_strobe",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) ),
          d_msg_1(msg_1), d_period_1(period_1), d_count_1(count_1), 
          d_msg_2(msg_2), d_period_2(period_2), d_count_2(count_2)
    {
      // output port
      d_out_port = pmt::mp("strobe");
      message_port_register_out(d_out_port);

      // input port, internal, GWNTimer writes here
      message_port_register_in(pmt::mp("timer_port"));
      set_msg_handler(pmt::mp("timer_port"),
          boost::bind(&gwntimer_strobe_impl::handle_timer_msg, this, _1));
      start_timer();
    }



    /* Our virtual destructor. */
    gwntimer_strobe_impl::~gwntimer_strobe_impl()
    { }



    /* Handles message sent by timer threads */
    void
    gwntimer_strobe_impl::handle_timer_msg(pmt::pmt_t pmt_msg) 
    {
      boost::mutex d_mutex;
      d_mutex.lock();
      process_data(pmt_msg);
      d_mutex.unlock();
    }



    /* Starts two timers */
    bool
    gwntimer_strobe_impl::start_timer()
    {
      tm_1 = new GWNTimer(this, 
        1, pmt::mp(d_msg_1), d_count_1, d_period_1);
      tm_2 = new GWNTimer(this, 
        2, pmt::mp(d_msg_2), d_count_2, d_period_2);
      return true;
    }


    /* Where actions happen */
    void
    gwntimer_strobe_impl::process_data(pmt::pmt_t pmt_tp_msg)
    {
      // unpack timer message: port, id_timer, counter, message
      std::string port = 
        pmt::symbol_to_string(pmt::tuple_ref(pmt_tp_msg, 0));
      long id_timer = 
        pmt::to_long(pmt::tuple_ref(pmt_tp_msg, 1));
      long counter = 
        pmt::to_long(pmt::tuple_ref(pmt_tp_msg, 2));
      std::string message = 
        pmt::symbol_to_string(pmt::tuple_ref(pmt_tp_msg, 3));
      std::cout << "  process_data: port " << port <<
        ", id_timer " << id_timer << ", counter " << counter <<
        ", message " << message << std::endl;
      message_port_pub(pmt::mp("strobe"), pmt_tp_msg);
    }


    /* TODO Mutually exclusive printing */
    void
    gwntimer_strobe_impl::mutex_prt(std::string msg)
    {
      boost::mutex d_mutex;
      d_mutex.lock();
      std::cout << msg << std::endl;
      d_mutex.unlock();
      return;
    }



    // failed attempt to make functions visble in QA
    /* TODO Timer reset, counter back to 0 in indicated timer */
    void
    gwntimer_strobe_impl::timer_reset(int timer_id)
    {
      if ( timer_id == 1 ) 
        tm_1->set_count(0);
      else if ( timer_id == 2 ) 
        tm_2->set_count(0);
      else
        return;
      std::cout << "    Timer " << timer_id 
        << " resetted to 0" << std::endl;
    }


  
    // failed attempt to make functions visble in QA
    /* TODO Interrupts indicated timer, should allow restart... */
    void
    gwntimer_strobe_impl::timer_interrupt(int timer_id, bool on_off)
    {
      if ( timer_id == 1 ) 
        tm_1->set_suspend(on_off);
      else if ( timer_id == 2 ) 
        tm_2->set_suspend(on_off);
      else
        return;
      std::cout << "    Timer " << timer_id 
        << " interrupt set to " << on_off << std::endl;
    }



  } /* namespace gwncppvgb */
} /* namespace gr */


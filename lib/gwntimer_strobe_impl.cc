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

    /** \brief GWNTimer, constructor

      @param p_block A pointer to the block having the timer.
      @param p_msg The message to emit periodically.
      @param p_count The number of times the message will be emitted.
      @param p_period: Tne period of emission in milliseconds.
    */
    gwntimer_strobe_impl::GWNTimer::GWNTimer(
      gwntimer_strobe_impl * p_block, 
      std::string p_msg, int p_count, float p_period_ms)
    {

      d_block = p_block;
      //pmt::pmt_t d_msg;
      d_msg = p_msg;
      d_count = p_count;
      d_counter = 0;
      d_period_ms = p_period_ms;

      d_finished = true;

      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind (
            &gwntimer_strobe_impl::GWNTimer::run_timer, this))
        );
    }  // end GWNTimer::GWNTimer


    /** \brief The function to run from the timer thread.
    */
    void
    gwntimer_strobe_impl::GWNTimer::run_timer()
    {

      boost::mutex d_mutex;

      while(!d_finished) {
        d_counter = d_counter + 1;
        if (d_counter > d_count)
        {
          d_finished = true;
          std::cout << "    === TIMER FINISHED: " << d_msg <<
            ", counter: " << d_counter << std::endl;
          break; //return;
        } // end if
        else
        {
          //d_block->post_timer_msg(d_msg);
          pmt::pmt_t pmt_port = pmt::mp("timer_port");
          pmt::pmt_t pmt_msg = pmt::mp( 
            d_msg + " msg nr " + std::to_string(d_counter));
          
          d_mutex.lock();
          d_block->gr::basic_block::_post(pmt_port, pmt_msg);
          d_mutex.unlock();

          //std::cout << "   timer message:" << d_msg << 
          //  ", counter: " << d_counter << std::endl;
          //d_block->process_data(d_msg);

          // sleep for d_period_ms milliseconds
          boost::this_thread::sleep( 
            boost::posix_time::milliseconds(d_period_ms));
        } // end else
      } // end while
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
          boost::bind(&gwntimer_strobe_impl::handle_msg, this, _1));
      start_timer();
    }


    /* Our virtual destructor. */
    gwntimer_strobe_impl::~gwntimer_strobe_impl()
    { }


    void
    gwntimer_strobe_impl::handle_msg(pmt::pmt_t pmt_msg) 
    {
      //std::string car = pmt::car(pmt_msg);
      //std::string msg = pmt::cdr(pmt_msg);
      //std::cout << "  timer1::handle_msg, message: " <<
      //  pmt::cdr(pmt_msg) << std::endl;
  
      boost::mutex d_mutex;
      d_mutex.lock();
      message_port_pub(pmt::mp("strobe"), pmt_msg);
      d_mutex.unlock();
    }


    bool
    gwntimer_strobe_impl::start_timer()
    {
      //std::string msg_1 = "message timer 1";
      //std::string msg_2 = "message timer 2";

      GWNTimer * tm_1;
      tm_1 = new GWNTimer(this, d_msg_1, d_count_1, d_period_1);
      GWNTimer * tm_2;
      tm_2 = new GWNTimer(this, d_msg_2, d_count_2, d_period_2);

      tm_1->set_finished(false);
      tm_2->set_finished(false);

      //return block::start_timer();
      return true;
    }

    void
    gwntimer_strobe_impl::process_data(std::string msg)
    {
      std::cout << "  process_data, timer message:" << msg << 
        std::endl;
    }


    void
    gwntimer_strobe_impl::mutex_prt(std::string msg)
    {
      boost::mutex d_mutex;
      d_mutex.lock();
      std::cout << msg << std::endl;
      d_mutex.unlock();
      return;
    }


  } /* namespace gwncppvgb */
} /* namespace gr */


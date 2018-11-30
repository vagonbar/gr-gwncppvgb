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
#include "gwnblock_dev_impl.h"

/* GWN inclusions */
#include <iostream>  // for string support
#include <string>    // for string concatenation
#include <sstream>   // for to_string
#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages

//#include <gwncppvgb/gwnblock_dev_pdata.h>  // process_data function
//#include <stdio.h>   // for printf


namespace gr {
  namespace gwncppvgb {

    /*
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */

    /* 
     * Block specific code, REWRITE for a new block.
     */


    void
    gwnblock_dev_impl::added_init() 
    {
      std::cout << "Added Initialization" << std::endl;
    
      d_debug = false;

      // set timers message, period, etc
      d_timers[0]->d_count = 5;
      d_timers[1]->d_count = 3;
      d_timers[0]->d_period_ms = 1000;
      d_timers[1]->d_period_ms = 1500;
      d_timers[0]->d_pmt_msg = pmt::mp("--- TIMER 0 AAAA");
      d_timers[1]->d_pmt_msg = pmt::mp("--- TIMER 1 BBBB");

      // start timers
      d_timers[0]->start_timer();
      d_timers[1]->start_timer();
    }



    pmt::pmt_t
    gwnblock_dev_impl::process_data(
      std::string p_port, pmt::pmt_t p_pmt_msg, int p_counter)
    {
      //d_block = p_block;
      std::string d_port = p_port;
      pmt::pmt_t d_pmt_msg = p_pmt_msg;
      int d_counter = p_counter;

      // alter message from timers, add counter
      if (d_port == "timer_0" || d_port == "timer_1")
      {
        std::string new_msg =  pmt::symbol_to_string(d_pmt_msg)
          + ", counter " + std::to_string(d_counter);
        d_pmt_msg = pmt::mp(new_msg);
      }

      if (d_debug) {
        std::cout << "...process_data, received message on port: " 
          << d_port << ", counter: " << d_counter << std::endl;
        std::cout << "...process_data, message: ";
        pmt::print(d_pmt_msg);
      }

      // emit message 
      pmt::pmt_t pmt_port = pmt::string_to_symbol("out_port_0");
      //pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev_proc); 
      return pmt::cons(pmt_port, d_pmt_msg);
    }




    /* 
     *  End block specific code.
     */


    /* GWNPort */
    gwnblock_dev_impl::GWNPort::GWNPort() {
      d_port = "";      // null port name
      d_port_nr = -1;   // first working port will be 0
      if (d_debug) {
        std::cout << "GWNPort, default constructor"
        << std::endl; }
    }  // end GWNPort

    std::string gwnblock_dev_impl::GWNPort::__str__() {
      std::string ss = "GWNPort name: " + d_port + 
        ", number: " + std::to_string(d_port_nr) + 
        ", in block: " + d_block->d_name + "\n";
      return ss;
    }


    /* GWNOutPort */
    gwnblock_dev_impl::GWNOutPort::
        GWNOutPort(gwnblock_dev_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort()
   {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNOutPort, constructor" << std::endl;
      }
    }  // end GWNOutPort


    /* GWNInPort */
    gwnblock_dev_impl::GWNInPort::
        GWNInPort(gwnblock_dev_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort()
    {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNInPort, constructor" << std::endl;
      }
    }  // end GWNInPort



    /* GWNTimer constructor */
    gwnblock_dev_impl::GWNTimer::GWNTimer(
      gwnblock_dev_impl * p_block, std::string p_id_timer, 
      pmt::pmt_t p_pmt_msg, int p_count, float p_period_ms) 
      : d_block(p_block), d_id_timer(p_id_timer),
        d_pmt_msg(p_pmt_msg), d_count(p_count),
        d_period_ms(p_period_ms)
    {
      d_counter = 0;
      d_suspend = false;  // always emits first message
    }  // end GWNTimer::GWNTimer



    /* GWNTimer, creates thread, timer starts immediately */
    void
    gwnblock_dev_impl::GWNTimer::start_timer()
    {
    
      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind (
            &gwnblock_dev_impl::GWNTimer::run_timer, this))
        );
      if (d_debug) {
      std::cout << "    === TIMER STARTED: " << d_id_timer <<
        ", count: " << d_count << std::endl;
      }
    }  // end GWNTimer::start_timer



    /* GWNTimer, runs timer, sleeps and sends message */
    void
    gwnblock_dev_impl::GWNTimer::run_timer()
    {
      boost::mutex d_mutex;

      while( d_counter < d_count) {
        d_counter = d_counter + 1;
        // first sleep for d_period_ms milliseconds
        boost::this_thread::sleep( 
          boost::posix_time::milliseconds(d_period_ms));
        if ( d_suspend == false ) // timer is not suspended
        {
          pmt::pmt_t pmt_id_timer = pmt::mp(d_id_timer);
          pmt::pmt_t pmt_id_counter = pmt::mp(d_counter);
          pmt::pmt_t pmt_timer_tuple = pmt::make_tuple( 
            pmt_id_timer, pmt_id_counter, d_pmt_msg);
          
          d_mutex.lock();
          d_block->gr::basic_block::_post(
            d_pmt_timer_port, pmt_timer_tuple);
          d_mutex.unlock();
        }  // end if
      } // end while

      if (d_debug) {
        std::cout << "   === TIMER FINISHED: " << d_id_timer <<
          ", counter: " << d_counter << 
          ", thread id: " << d_thread->get_id() << std::endl;
      }
      d_thread->interrupt();  // end thread
      return;
 
    } // end run_timer()



    /* Handles message sent by timer threads */
    void
    gwnblock_dev_impl::handle_timer_msg(pmt::pmt_t p_pmt_timer) 
    {
      std::string id_timer = pmt::symbol_to_string(
        pmt::tuple_ref(p_pmt_timer, 0));
      int counter = pmt::to_long(pmt::tuple_ref(p_pmt_timer, 1));
      pmt::pmt_t pmt_msg = pmt::tuple_ref(p_pmt_timer, 2);
      //std::cout << " In handle_timer_msg" << std::endl;
      boost::mutex d_mutex;
      d_mutex.lock();
      //pmt::pmt_t pmt_port_msg = pdata_obj->process_data(id_timer, pmt_msg, counter);
      pmt::pmt_t pmt_port_msg = process_data(id_timer, pmt_msg, counter);
      d_mutex.unlock();

      pmt::pmt_t pmt_port_send = pmt::car(pmt_port_msg);
      pmt::pmt_t pmt_msg_send = pmt::cdr(pmt_port_msg);
      post_message(pmt_port_send, pmt_msg_send);

    }  // end handle_timer_msg




    /* *** GWN gwnblock_dev *** */

    /* GWN gwnblock_dev attributes and functions */

    /*std::string gwnblock_dev_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->d_name; 
      return ss;
    }*/


    /* Handles messages received on message input ports. */
    void gwnblock_dev_impl::handle_msg (pmt::pmt_t pmt_msg)
    {
      std::string in_msg = pmt::symbol_to_string(pmt_msg);
      if (d_debug) { 
        std::cout << "...handle input msg: " << in_msg << std::endl;
      }

      pmt::pmt_t pmt_port_msg = 
        //pdata_obj->process_data("handle_msg", pmt_msg, 0);
        process_data("handle_msg", pmt_msg, 0);

      pmt::pmt_t pmt_port_send = pmt::car(pmt_port_msg);
      pmt::pmt_t pmt_msg_send = pmt::cdr(pmt_port_msg);
      post_message(pmt_port_send, pmt_msg_send);
    }  // end handle_msg


    void gwnblock_dev_impl::post_message(pmt::pmt_t pmt_port,
        pmt::pmt_t pmt_msg_send)
    {
      if (d_debug) {
        std::cout << "...post_message, sent: <" 
          << pmt::symbol_to_string(pmt_msg_send) << 
          "> on port: " << pmt::symbol_to_string(pmt_port) << std::endl;
      }
      boost::mutex d_mutex;
      d_mutex.lock();
      message_port_pub(pmt_port, pmt_msg_send);
      d_mutex.unlock();
    }  // end post_message



    /* GNU Radio defaults for block construction */
    gwnblock_dev::sptr
    gwnblock_dev::make(std::string message, int counter)
    {
      return gnuradio::get_initial_sptr
        (new gwnblock_dev_impl(message, counter));
    }  // end make



    /* gwnblock_dev: the private constructor */
    gwnblock_dev_impl::gwnblock_dev_impl(std::string message, int counter)
      : gr::block("gwnblock_dev",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) )
    {
      // GWN block name, ports and timers as block attributes
      d_name = "gwnblock_dev";
      d_number_in = 1;
      d_number_out = 1;
      d_number_timers = 2;

      // GWN user arguments initialization
      d_message = message;
      d_counter = counter;
      d_debug = true;
      //pdata_obj = new gwnblock_dev_pdata(message, counter);

      if (d_debug) {
        std::cout << "gwnblock_dev, constructor, name " << 
          d_name << ", number_in " << d_number_in << 
          ", number_out " << d_number_out << std::endl;
      }

      // gwnblock_dev, create out ports
      int i;
      d_ports_out.resize(d_number_out);
      std::string out_port;
      pmt::pmt_t pmt_out_port;

      GWNOutPort * out_port_new;
      for ( i=0; i < d_number_out; i++) {
        out_port = "out_port_" + std::to_string(i);
        out_port_new = new GWNOutPort(this, out_port, i);
        d_ports_out[i] = out_port_new;
        pmt_out_port = pmt::string_to_symbol(out_port);
        message_port_register_out(pmt_out_port); 
      }  // end for
      if (d_debug) {    // print items in vector of out ports
        std::cout << "=== gwnblock_dev, out ports:" << std::endl;
        for ( i=0; i < d_number_out; i++) {
          std::cout << "  out port " << i << 
            ": " << d_ports_out[i]->__str__(); // << std::endl; 
        }
      }

      // gwnblock_dev, create in ports
      //int i;                  // already declared
      d_ports_in.resize(d_number_in);
      std::string in_port;  
      pmt::pmt_t pmt_in_port;

      GWNInPort * in_port_new;
      for ( i=0; i < d_number_in; i++) {
        in_port = "in_port_" + std::to_string(i);
        in_port_new = new GWNInPort(this, in_port, i);
        d_ports_in[i] = in_port_new;
        pmt_in_port = pmt::string_to_symbol(in_port);

        message_port_register_in(pmt_in_port);
        set_msg_handler(pmt_in_port,
          boost::bind(&gwnblock_dev_impl::handle_msg, this, _1));
      }  // end for
      if (d_debug) {      // print items in vector of in ports
        std::cout << "=== gwnblock_dev, in ports:" << std::endl;
        for ( i=0; i < d_number_in; i++) {
          std::cout << "  in port " << i << 
            ": " << d_ports_in[i]->__str__(); // << std::endl; 
        }
      }    // end if


      // register the timer port
      message_port_register_in(pmt::mp("timer_port"));
      set_msg_handler(pmt::mp("timer_port"),
        boost::bind(&gwnblock_dev_impl::handle_timer_msg, 
        this, _1));
      // create timers
      d_timers.resize(d_number_timers);
      GWNTimer * timer_new;
      for ( i=0; i< d_number_timers; i++) {
        timer_new = new GWNTimer(this, 
          "timer_" + std::to_string(i), 
          pmt::mp("--- An internal TIMER message"), 0, 10000);
          //pmt::mp("--- An internal TIMER message"), 5, 1000);
        d_timers[i] = timer_new;
        //timer_new->start_timer();  // moved to added_init
      }  // end for


      // additional initialization
      //gwnblock_dev_impl::added_init();
      added_init();

    }  // end constructor


    /* gwnblock_dev: our virtual destructor */
    gwnblock_dev_impl::~gwnblock_dev_impl()
    { }


  } /* namespace gwncppvgb */
} /* namespace gr */


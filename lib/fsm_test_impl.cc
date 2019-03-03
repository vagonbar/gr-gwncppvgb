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
#include "fsm_test_impl.h"

/* GWN inclusions */
#include <iostream>  // for string support
#include <string>    // for string concatenation
#include <sstream>   // for to_string
#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages



namespace gr {
  namespace gwncppvgb {

    /*
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */


    /* ********************************************** 
     * Block specific code, REWRITE for a new block.
     ************************************************/

    /* Additional initialization, REWRITE as desired. */
    void
    fsm_test_impl::added_init() 
    {
    
      // GWN TAG include optional FSM 
      //d_fsm = new gwnfsm_dev("INIT");
      d_fsm = new fsmblk("INIT");
      if (d_debug)
      {
        d_fsm->print_transitions();
        std::cout << d_fsm->show_state();
      }

      // set timers message, period, etc
      // no timers in this block

    }


    /* Timer and input messages processing, REWRITE as desired. */
    void
    fsm_test_impl::process_data(
      std::string port, pmt::pmt_t pmt_msg)
    {
      std::string d_port = port;
      std::string fsm_symbol;

      // verify if message is dictionary (GWN) or other (GR)
      if ( pmt::is_dict(pmt_msg) )
      {
        // GWN message, unpack type, subtype, seq_nr, FSM symbol
        std::string type = pmt::symbol_to_string (pmt::dict_ref(
          pmt_msg, pmt::intern("type"), pmt::PMT_NIL));
        std::string subtype = pmt::symbol_to_string (pmt::dict_ref(
          pmt_msg, pmt::intern("subtype"), pmt::PMT_NIL));
        int seq_nr = pmt::to_long (pmt::dict_ref(
          pmt_msg, pmt::intern("seq_nr"), pmt::PMT_NIL)); 
        fsm_symbol = pmt::symbol_to_string (pmt::dict_ref(
          pmt_msg, pmt::intern("symbol"), pmt::PMT_NIL));
        
        if ( type == "Symbol" )  
        {           // GWN message
          if (d_debug) {
            std::cout << "...process_data, SYMBOL msg from " <<
              d_port << std::endl << "   ";
            std::cout << "    type: " << type << ", subtype: " <<
              subtype << ", seq_nr: " << seq_nr <<
              ", symbol: " << fsm_symbol <<  std::endl;
            pmt::print(pmt_msg);
          }
        } else {
          // actions on other message type
        }
      } else {
        // actions on non GWN message
        if (d_debug) {
          std::cout << "    process_data, msg from " <<
            d_port << ":  ";
          pmt::print(pmt_msg);
        }
      }  // end message type

      // invoke FSM with symbol
      std::string result = "";    // FSM action function result
      result = d_fsm->process(fsm_symbol, "", "");
      // emit FSM messages on output port
      pmt::pmt_t pmt_port = pmt::string_to_symbol("out_port_0");
      pmt_msg = pmt::mp(result);
      post_message(pmt_port, pmt_msg);

      if (d_debug) {
        std::cout << d_fsm->show_state();
      }
    }


    /* ************************* 
     *  End block specific code.
     ***************************/



    /* GWNPort */
    fsm_test_impl::GWNPort::GWNPort() {
      d_port = "";      // null port name
      d_port_nr = -1;   // first working port will be 0
      d_debug = false;
      if (d_debug) {
        std::cout << "GWNPort, default constructor"
        << std::endl; }
    }  // end GWNPort

    std::string fsm_test_impl::GWNPort::__str__() {
      std::string ss = "GWNPort name: " + d_port + 
        ", number: " + std::to_string(d_port_nr) + 
        ", in block: " + d_block->d_name + "\n";
      return ss;
    } 


    /* GWNOutPort */
    fsm_test_impl::GWNOutPort::
        GWNOutPort(fsm_test_impl * p_block, 
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
    fsm_test_impl::GWNInPort::
        GWNInPort(fsm_test_impl * p_block, 
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
    fsm_test_impl::GWNTimer::GWNTimer(
      fsm_test_impl * block, std::string id_timer, 
      pmt::pmt_t pmt_msg, int count, float period_ms) 
      : d_block(block), d_id_timer(id_timer),
        d_pmt_msg(pmt_msg), d_count(count),
        d_period_ms(period_ms)
    {
      d_counter = 0;
      d_suspend = false;  // always emits first message
      d_debug = false;
    }  // end GWNTimer::GWNTimer



    /* GWNTimer, creates thread, timer starts immediately */
    void
    fsm_test_impl::GWNTimer::start_timer()
    {
    
      d_thread = boost::shared_ptr<gr::thread::thread>
        (new gr::thread::thread(boost::bind (
            &fsm_test_impl::GWNTimer::run_timer, this))
        );
      if (d_debug) {
        std::cout << "    === TIMER STARTED: " << d_id_timer <<
          ", count: " << d_count << std::endl;
      }
    }  // end GWNTimer::start_timer



    /* GWNTimer, runs timer, sleeps and sends message */
    void
    fsm_test_impl::GWNTimer::run_timer()
    {
      boost::mutex d_mutex;

      while( d_counter < d_count) {
        d_counter = d_counter + 1;
        // first sleep for d_period_ms milliseconds
        boost::this_thread::sleep( 
          boost::posix_time::milliseconds(d_period_ms));
        if ( d_suspend == false ) // timer is not suspended
        {
          // make timer message
          pmt::pmt_t pmt_timer_dict = pmt::make_dict();
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("type"), pmt::intern("Timer")); 
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("subtype"), pmt::intern(d_id_timer));
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("seq_nr"), pmt::from_long(d_counter));
          pmt_timer_dict = pmt::dict_add(pmt_timer_dict, 
            pmt::intern("message"), d_pmt_msg);
          // mutex lock, post message, unlock
          d_mutex.lock();
          d_block->gr::basic_block::_post(
            d_pmt_timer_port, pmt_timer_dict);
          d_mutex.unlock();
        }  // end if
      } // end while

      if (d_debug) {
        std::cout << "    === TIMER FINISHED: " << d_id_timer <<
          ", counter: " << d_counter << 
          ", thread id: " << d_thread->get_id() << std::endl;
      }
      d_thread->interrupt();  // end thread
      return;
 
    } // end run_timer()



    /* Handles message sent by timer threads */
    void
    fsm_test_impl::handle_timer_msg(pmt::pmt_t pmt_msg) 
    {
      std::string timer_id = pmt::symbol_to_string( pmt::dict_ref (
        pmt_msg, pmt::intern("subtype"), pmt::PMT_NIL));
      // mutex lock, invoke process_data, unlock
      boost::mutex d_mutex;
      d_mutex.lock();
      process_data(timer_id, pmt_msg);
      d_mutex.unlock();
    }  // end handle_timer_msg



    /* Handles messages received on message input ports. */
    void fsm_test_impl::handle_msg (pmt::pmt_t pmt_msg)
    {
      if (d_debug) { 
        std::cout << "...handle input msg: ";
        pmt::print(pmt_msg);
      } 
      // mutex lock, invoke process_data, unlock
      boost::mutex d_mutex;
      d_mutex.lock();
        process_data("handle_msg", pmt_msg); // 0);
      d_mutex.unlock();
    }  // end handle_msg



    /* post_message in PMT formatted port */
    void fsm_test_impl::post_message(pmt::pmt_t pmt_port,
        pmt::pmt_t pmt_msg_send)
    {
      if (d_debug) {
        std::cout << "...post_message on port: " << 
          pmt::symbol_to_string(pmt_port) << std::endl;
        pmt::print(pmt_msg_send);  
      }
      boost::mutex d_mutex;
      d_mutex.lock();
      message_port_pub(pmt_port, pmt_msg_send);
      d_mutex.unlock();
    }  // end post_message



    /* post_message in string formatted port */
    void fsm_test_impl::post_message(std::string port,
        pmt::pmt_t pmt_msg)
    {
      pmt::pmt_t pmt_port = pmt::intern(port); 
      post_message(pmt_port, pmt_msg);
    }



    /* GNU Radio defaults for block construction */
    fsm_test::sptr
    fsm_test::make(bool debug)
    {
      return gnuradio::get_initial_sptr
        (new fsm_test_impl (debug) ); 
    }  // end make



    /* fsm_test: the private constructor */
    fsm_test_impl::fsm_test_impl 
      (bool debug) 
      : gr::block("fsm_test",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) ) //,
      // GWN TAG user arguments constructor init
      , d_debug(debug)
      

    {
      // GWN block name, ports and timers as block attributes
      d_name = "fsm_test";
      d_number_in = 1;
      d_number_out = 1;
      d_number_timers = 0;

      // GWN user arguments initialization
      d_debug = debug; 

      if (d_debug) {
        std::cout << "fsm_test, constructor, name " << 
          d_name << ", number_in " << d_number_in << 
          ", number_out " << d_number_out << 
          ", debug: " << d_debug << std::endl;
      }

      // fsm_test, create out ports
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
        std::cout << "=== fsm_test, out ports:" << std::endl;
        for ( i=0; i < d_number_out; i++) {
          std::cout << "  out port " << i << 
            ": " << d_ports_out[i]->__str__(); // << std::endl; 
        }
      }

      // fsm_test, create in ports
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
          boost::bind(&fsm_test_impl::handle_msg, this, _1));
      }  // end for
      if (d_debug) {      // print items in vector of in ports
        std::cout << "=== fsm_test, in ports:" << std::endl;
        for ( i=0; i < d_number_in; i++) {
          std::cout << "  in port " << i << 
            ": " << d_ports_in[i]->__str__(); // << std::endl; 
        }
      }    // end if


      // register the timer port
      message_port_register_in(pmt::mp("timer_port"));
      set_msg_handler(pmt::mp("timer_port"),
        boost::bind(&fsm_test_impl::handle_timer_msg, 
        this, _1));
      // create timers
      d_timers.resize(d_number_timers);
      GWNTimer * timer_new;
      for ( i=0; i< d_number_timers; i++) {
        timer_new = new GWNTimer(this, 
          "timer_" + std::to_string(i), 
          pmt::mp("--- An internal TIMER message"), 0, 10000);
        d_timers[i] = timer_new;
      }  // end for


      // additional initialization
      added_init();

    }  // end constructor


    /* fsm_test: our virtual destructor */
    fsm_test_impl::~fsm_test_impl()
    { }


  } /* namespace gwncppvgb */
} /* namespace gr */


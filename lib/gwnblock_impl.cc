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
#include "gwnblock_impl.h"

// GWN additions
#include <sstream>   // for to_string

#include <iostream>  // for string support
#include <string>    // for string concatenation

#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages

#include <stdio.h>   // for printf

namespace gr {
  namespace gwncppvgb {


    template <typename T>
    std::string to_string(T value)
    {
      //create an output string stream
      std::ostringstream os ;

      //throw the value into the string stream
      os << value ;

      //convert the string stream into a string and return
      return os.str() ;
    }


    bool debug = true;    // if true prints debug messages
    void set_debug(bool dbg) {
      debug = dbg;
      std::cout << "Debug flag set to " << debug << std::endl;
    }


    /* GWNPort */

    GWNPort::GWNPort() {
      port = "";      // null port name
      port_nr = -1;   // first working port will be 0
      if (debug) { std::cout << "GWNPort, default constructor"
        << std::endl; }
    }

    std::string GWNPort::__str__() {
      std::string ss = "GWNPort port name: " + port + 
        ", port number: " + to_string(port_nr) + 
        ", in block name: " + block->name + "\n";
      return ss;
    }



    /* GWNOutPort */

    //GWNOutPort::GWNOutPort() : GWNPort() {}

    GWNOutPort::GWNOutPort(gwnblock_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      block = p_block;
      port = p_port;
      port_nr = p_port_nr;
      if (debug) {
        std::cout << "GWNOutPort, constructor" << std::endl;
      }
    }

    /*
    void GWNOutPort::post_message(std::string ev) {
      // receives string, creates PMT, outputs PMT message
      if (debug) { 
        std::cout << "GWNOutPort, post_message, port: " << 
          port_nr << " in block: " << block->name << std::endl;
        std::cout << "...message: " << ev << std::endl;
      }
      pmt::pmt_t pmt_port = pmt::string_to_symbol(port);
      pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev); 
      //block->message_port_pub(pmt_port, pmt_msg);
      //block->message_port_pub(pmt::mp("out_port_0"), pmt::mp(ev));
    } */

       

    /* GWNInPort */

    //GWNInPort::GWNInPort() : GWNPort() {}

    GWNInPort::GWNInPort(gwnblock_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      block = p_block;
      port = p_port;
      port_nr = p_port_nr;
      if (debug) {
        std::cout << "GWNInPort, constructor" << std::endl;
      }
    }



    /* gwnblock */

    gwnblock::sptr
    gwnblock::make(std::string name, int number_in, int number_out, int number_timers, int number_timeouts)
    {
      return gnuradio::get_initial_sptr
        (new gwnblock_impl(name, number_in, number_out, number_timers, number_timeouts));
    }


    std::string gwnblock_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->name; 
      return ss;
    }


    void gwnblock_impl::handle_msg (pmt::pmt_t pmt_msg) {
      std::string ev = pmt::symbol_to_string(pmt_msg);
      if (debug) { 
        std::cout << "Handle msg, received: " << ev << std::endl;
      }
      process_data(ev);
    } 

    
    void gwnblock_impl::post_message(std::string port,
        std::string ev) {
      if (debug) {
        std::cout << "Post message, sent: " 
          << ev << std::endl;
      }
      pmt::pmt_t pmt_port = pmt::string_to_symbol(port);
      pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev); 
      message_port_pub(pmt_port, pmt_msg);
    }

    void gwnblock_impl::process_data(std::string ev) {
      // to be rewritten in descedant class
      if (debug) {
        std::cout << "Process_data, received and sent: " << ev << std::endl;
        //std::cout << ports_out[0]->__str__() << std::endl;
        //std::cout << ports_out[0]->port << std::endl;
        post_message("out_port_0", ev);
        //ports_out[0]->post_message(ev);
        //message_port_pub(pmt::mp("out_port_0"), pmt::mp(ev));
      }
    }



    /* gwnblock: the private constructor */

    gwnblock_impl::gwnblock_impl(std::string p_name,
      int p_number_in, int p_number_out, int p_number_timers,
      int p_number_timeouts)
      : gr::block("gwnblock",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) )
    {
      // different parameter and member names required
      // for correct initialization of member
      name = p_name;
      number_in = p_number_in;
      number_out = p_number_out;
      number_timers = p_number_timers;
      number_timeouts = p_number_timeouts;

      if (debug) {
        std::cout << "gwnblock, constructor, name " << 
          name << ", number_in " << number_in << 
          ", number_out " << number_out << std::endl;
      }


      // gwnblock::set_out_size (int number_out)
      ports_out.resize(number_out);
      int i;
      std::string out_port;
      std::string message;
      pmt::pmt_t pmt_out_port;

      for ( i=0; i < number_out; i++) {
        out_port = "out_port_" + to_string(i);
        GWNOutPort port_new = GWNOutPort(this, out_port, i);
        ports_out[i] = &port_new;
        pmt_out_port = pmt::string_to_symbol(out_port);
        message_port_register_out(pmt_out_port); 
        if (debug) {
          std::cout << ports_out[i]->__str__() << std::endl; 
          std::string dbg_msg = "Post Message debug\n...";
          dbg_msg += ports_out[i]->__str__();
          std::cout << dbg_msg << std::endl;
        }
      }  


      // gwnblock::set_n_size (int number_in)
      //int i;                  // already declared
      ports_in.resize(number_in);
      std::string in_port;  
      //std::string message;    // already declared
      pmt::pmt_t pmt_in_port;

      for ( i=0; i < number_in; i++) {
        in_port = "in_port_" + to_string(i);
        GWNInPort port_new = GWNInPort(this, in_port, i);
        ports_in[i] = &port_new;
        pmt_in_port = pmt::string_to_symbol(in_port);
        message_port_register_in(pmt_in_port);
        set_msg_handler(pmt_in_port,
          boost::bind(&gwnblock_impl::handle_msg, this, _1));
        if (debug) {
          std::cout << ports_in[i]->__str__() << std::endl; 
        } 
      } 
    }


    /* gwnblock: our virtual destructor */
    gwnblock_impl::~gwnblock_impl()
    {
    }




    /* GNU Radio */

    void
    gwnblock_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    gwnblock_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const int *in = (const int *) input_items[0];
      int *out = (int *) output_items[0];

      // Do <+signal processing+>

      std::cout << "gwnblock, in work() function" << std::endl;

      // Tell runtime system how many input items we consumed on
      // each input stream.


      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


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
#include "message_passer_impl.h"

/* GWN inclusions */
#include <iostream>  // for string support
#include <string>    // for string concatenation
#include <sstream>   // for to_string
#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages
#include <gwncppvgb/message_passer_pdata.h>  // process_data function
//#include <stdio.h>   // for printf


namespace gr {
  namespace gwncppvgb {

    /* A template for conversion of types to string */
    template <typename T>
    std::string to_string(T value)
    {
      std::ostringstream os ;
      os << value ;
      return os.str() ;
    }


    /* GWNPort */
    message_passer_impl::GWNPort::GWNPort() {
      d_port = "";      // null port name
      d_port_nr = -1;   // first working port will be 0
      if (d_debug) { std::cout << "GWNPort, default constructor"
        << std::endl; }
    }
    std::string message_passer_impl::GWNPort::__str__() {
      std::string ss = "GWNPort name: " + d_port + 
        ", number: " + to_string(d_port_nr) + 
        ", in block: " + d_block->d_name + "\n";
      return ss;
    }


    /* GWNOutPort */
    message_passer_impl::GWNOutPort::GWNOutPort(message_passer_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNOutPort, constructor" << std::endl;
      }
    }


    /* GWNInPort */
    message_passer_impl::GWNInPort::GWNInPort(message_passer_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNInPort, constructor" << std::endl;
      }
    }



    /* GWN message_passer */

    /* GNU Radio defaults for block construction */
    message_passer::sptr
    message_passer::make(std::string name, int number_in, int number_out, int number_timers, int number_timeouts)
    {
      return gnuradio::get_initial_sptr
        (new message_passer_impl(name, number_in, number_out, number_timers, 
          number_timeouts));
    }


    /* GWN message_passer attributes and functions */

    std::string message_passer_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->d_name; 
      return ss;
    }

    void message_passer_impl::handle_msg (pmt::pmt_t pmt_msg) {
      std::string ev = pmt::symbol_to_string(pmt_msg);
      if (d_debug) { 
        std::cout << "Handle msg, received: " << ev << std::endl;
      }
      //process_data(ev);
      std::string n_ev = message_passer_pdata::process_data(ev);
      post_message("out_port_0", n_ev);
    } 

    void message_passer_impl::post_message(std::string port,
        std::string ev) {
      if (d_debug) {
        std::cout << "Post message, sent: " 
          << ev << std::endl;
      }
      pmt::pmt_t pmt_port = pmt::string_to_symbol(port);
      pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev); 
      message_port_pub(pmt_port, pmt_msg);
    }



    /* message_passer: the private constructor */

    message_passer_impl::message_passer_impl(std::string p_name,
      int p_number_in, int p_number_out, int p_number_timers,
      int p_number_timeouts)
      : gr::block("message_passer",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) )
    {
      d_name = p_name;
      d_number_in = p_number_in;
      d_number_out = p_number_out;
      d_number_timers = p_number_timers;
      d_number_timeouts = p_number_timeouts;
      d_debug = true;

      if (d_debug) {
        std::cout << "message_passer, constructor, name " << 
          d_name << ", number_in " << d_number_in << 
          ", number_out " << d_number_out << std::endl;
      }

      // message_passer, create out ports
      int i;
      std::string message;
      d_ports_out.resize(d_number_out);
      std::string out_port;
      pmt::pmt_t pmt_out_port;

      GWNOutPort * out_port_new;
      for ( i=0; i < d_number_out; i++) {
        out_port = "out_port_" + to_string(i);
        out_port_new = new GWNOutPort(this, out_port, i);
        d_ports_out[i] = out_port_new;
        pmt_out_port = pmt::string_to_symbol(out_port);
        if (d_debug) {
          std::cout << "...about to register out_port" << std::endl;}
        message_port_register_out(pmt_out_port); 
        if (d_debug) {
          std::cout << "...registered out_port" << std::endl;}
      }  
      if (d_debug) {    // print items in vector of out ports
        std::cout << "=== message_passer, out ports:" << std::endl;
        for ( i=0; i < d_number_out; i++) {
          std::cout << "  out port " << i << 
            ": " << d_ports_out[i]->__str__(); // << std::endl; 
          //std::string dbg_msg = "Post Message debug\n...";
          //dbg_msg += ports_out[i]->__str__();
          //std::cout << dbg_msg << std::endl;
        }
      }

      // message_passer, create in ports
      //int i;                  // already declared
      //std::string message;    // already declared
      d_ports_in.resize(d_number_in);
      std::string in_port;  
      pmt::pmt_t pmt_in_port;

      GWNInPort * in_port_new;  // requires default constructor
      for ( i=0; i < d_number_in; i++) {
        in_port = "in_port_" + to_string(i);
        in_port_new = new GWNInPort(this, in_port, i);
        d_ports_in[i] = in_port_new;
        pmt_in_port = pmt::string_to_symbol(in_port);

        if (d_debug) {
          std::cout << "...about to register in_port" << std::endl;}
        message_port_register_in(pmt_in_port);
        if (d_debug) {
          std::cout << "...registered in_port" << std::endl;}
        set_msg_handler(pmt_in_port,
          boost::bind(&message_passer_impl::handle_msg, this, _1));
      } 
      if (d_debug) {      // print items in vector of in ports
        std::cout << "=== message_passer, in ports:" << std::endl;
        for ( i=0; i < d_number_in; i++) {
          std::cout << "  in port " << i << 
            ": " << d_ports_in[i]->__str__(); // << std::endl; 
        }
      }

      if (d_debug) {
        std::cout << "=== message_passer, receive and send test" <<
          std::endl; }
    }


    /* message_passer: our virtual destructor */
    message_passer_impl::~message_passer_impl()
    {
    }



    /* GNU Radio  defaults */

    void
    message_passer_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    message_passer_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const int *in = (const int *) input_items[0];
      int *out = (int *) output_items[0];
      // Do <+signal processing+>
      std::cout << "message_passer, in work() function" << std::endl;
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


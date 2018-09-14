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
#include "gwnblockc_impl.h"

/* GWN inclusions */
#include <iostream>  // for string support
#include <string>    // for string concatenation
#include <sstream>   // for to_string
#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages
#include <gwncppvgb/gwnblockc_pdata.h>  // process_data function
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
    gwnblockc_impl::GWNPort::GWNPort() {
      d_port = "";      // null port name
      d_port_nr = -1;   // first working port will be 0
      if (d_debug) { std::cout << "GWNPort, default constructor"
        << std::endl; }
    }
    std::string gwnblockc_impl::GWNPort::__str__() {
      std::string ss = "GWNPort name: " + d_port + 
        ", number: " + to_string(d_port_nr) + 
        ", in block: " + d_block->d_name + "\n";
      return ss;
    }


    /* GWNOutPort */
    gwnblockc_impl::GWNOutPort::GWNOutPort(gwnblockc_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNOutPort, constructor" << std::endl;
      }
    }


    /* GWNInPort */
    gwnblockc_impl::GWNInPort::GWNInPort(gwnblockc_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      d_block = p_block;
      d_port = p_port;
      d_port_nr = p_port_nr;
      if (d_debug) {
        std::cout << "GWNInPort, constructor" << std::endl;
      }
    }



    /* *** GWN gwnblockc *** */

    /* GWN gwnblockc attributes and functions */

    std::string gwnblockc_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->d_name; 
      return ss;
    }

    void gwnblockc_impl::handle_msg (pmt::pmt_t pmt_msg) {
      std::string ev = pmt::symbol_to_string(pmt_msg);
      if (d_debug) { 
        std::cout << "Handle msg, received: " << ev << std::endl;
      }
      pmt::pmt_t port_ev = pdata_obj->process_data(ev);
      pmt::pmt_t pmt_port_send = pmt::car(port_ev);
      pmt::pmt_t pmt_msg_send = pmt::cdr(port_ev);
      post_message(pmt_port_send, pmt_msg_send);
    } 

    void gwnblockc_impl::post_message(pmt::pmt_t pmt_port,
        pmt::pmt_t pmt_msg_send) {
      if (d_debug) {
        std::cout << "Post message, sent: <" 
          << pmt::symbol_to_string(pmt_msg_send) << 
          "> on port: " << pmt::symbol_to_string(pmt_port) << std::endl;
      }
      message_port_pub(pmt_port, pmt_msg_send);
    }



    /* GNU Radio defaults for block construction */
    gwnblockc::sptr
    gwnblockc::make(<GWN user arguments list>)
    {
      return gnuradio::get_initial_sptr
        (new gwnblockc_impl(<GWN user parameters list>));
    }


    /* gwnblockc: the private constructor */

    gwnblockc_impl::gwnblockc_impl(<GWN user arguments list>)
      : gr::block("gwnblockc",
              gr::io_signature::make(0, 0, sizeof(int)),
              gr::io_signature::make(0, 0, sizeof(int)) )
    {
      // GWN block name, ports and timers as block attributes
      d_name = "no_name";
      d_number_in = 0;
      d_number_out = 0;
      d_number_timers = 0;

      // GWN user arguments initialization

      d_debug = true;
      pdata_obj = new gwnblockc_pdata(<GWN user parameters list>);


      if (d_debug) {
        std::cout << "gwnblockc, constructor, name " << 
          d_name << ", number_in " << d_number_in << 
          ", number_out " << d_number_out << std::endl;
      }

      // gwnblockc, create out ports
      int i;
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
        std::cout << "=== gwnblockc, out ports:" << std::endl;
        for ( i=0; i < d_number_out; i++) {
          std::cout << "  out port " << i << 
            ": " << d_ports_out[i]->__str__(); // << std::endl; 
          //std::string dbg_msg = "Post Message debug\n...";
          //dbg_msg += ports_out[i]->__str__();
          //std::cout << dbg_msg << std::endl;
        }
      }

      // gwnblockc, create in ports
      //int i;                  // already declared
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
          boost::bind(&gwnblockc_impl::handle_msg, this, _1));
      } 
      if (d_debug) {      // print items in vector of in ports
        std::cout << "=== gwnblockc, in ports:" << std::endl;
        for ( i=0; i < d_number_in; i++) {
          std::cout << "  in port " << i << 
            ": " << d_ports_in[i]->__str__(); // << std::endl; 
        }
      }

      if (d_debug) {
        std::cout << "=== gwnblockc, receive and send test" <<
          std::endl; }
    }


    /* gwnblockc: our virtual destructor */
    gwnblockc_impl::~gwnblockc_impl()
    {
    }



    /* GNU Radio  defaults */

    void
    gwnblockc_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    gwnblockc_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      const int *in = (const int *) input_items[0];
      int *out = (int *) output_items[0];
      // Do <+signal processing+>
      std::cout << "gwnblockc, in work() function" << std::endl;
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);
      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


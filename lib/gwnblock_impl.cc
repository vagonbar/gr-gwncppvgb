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
#include<sstream>   // for to_string

#include <pmt/pmt.h>              // for messages
#include <gnuradio/blocks/pdu.h>  // for messages

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

    /* GWNPort */

    GWNPort::GWNPort() {
      //block = void pointer, will receive pointer to gwnblock
      //block = nullptr; // necessary, possible?
      port = "";      // null port name
      port_nr = -1;   // first working port will be 0
      if (debug) { std::cout << "GWNPort, default constructor"
        << std::endl; }
    }

    std::string GWNPort::__str__() {
      std::string ss = "__str__ string to return";
      if (debug) {
        std::cout << "GWNPort, port name: " << port << 
          ", port number: " << port_nr << ", in block name: " <<
          block->name << std::endl;
      }
      return ss;
    }


    /* GWNOutPort */

    GWNOutPort::GWNOutPort(gwnblock_impl * p_block, 
        std::string p_port, int p_port_nr) : GWNPort() {
      block = p_block;
      port = p_port;
      port_nr = p_port_nr;
      /*if (debug) { 
         std::cout << "GWNOutPort, constructor, block name: " <<
          block->name << std::endl;} */
    }


    GWNOutPort::GWNOutPort() : GWNPort() {
       if (debug) { 
         std::cout << "GWNOutPort, default constructor" << 
           std::endl;}
    }

    void GWNOutPort::post_message(std::string ev) {
      // string instead of ev, for debug in development
      if (debug) { 
        std::cout << "GWNOutPort, post_message, port: " << 
          port_nr << " in block: " << block->name << std::endl;
        std::cout << "...message: " << ev << std::endl;
      }

      // send message on output port
      std::string ev_str = ev;     // transmit strings, for now

      //pmt::pmt_t pmt_msg = pmt::intern(ev_str);

      // from Python gwnblock, pmt:_mp from message_strobe
      /*pmt::pmt_t pmt_msg = pmt::cons(pmt::PMT_NIL, 
        pmt::mp(ev_str) );
      pmt::pmt_t pmt_port = pmt::mp(port);
      block->message_port_pub(pmt_port, pmt::cons(pmt::PMT_NIL, pmt_msg) ); */
      
      // from gwncpp  
      //block->gr::basic_block::message_port_pub (pmt_port, pmt_msg);
      //block->gr::basic_block::_post (pmt_port, pmt_msg );

      // from message_strobe
      block->message_port_pub (pmt::mp(port), pmt::mp(ev_str));

    }
       


    /* gwnblock */

    gwnblock::sptr
    gwnblock::make(std::string name, int number_in, int number_out, int number_timers, int number_timeouts)
    {
      return gnuradio::get_initial_sptr
        (new gwnblock_impl(name, number_in, number_out, number_timers, number_timeouts));
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


      // for early stage debug
      if (debug) { 
        std::cout << "gwnblock, private constructor; " <<
        "block name: " << this->name << std::endl; 
      }

      // set_out_size, now inline, convert to function,
      // must solve visibility of GWNOutPort in function

      // cannot assign, maybe use <vector>
      //GWNOutPort * ports_out_ar [number_out];    // local variable
      //ports_out = ports_out_ar;                // member variable

      int i;
      std::string out_port;
      std::string message;

      pmt::pmt_t pmt_out_port;

      for ( i=0; i < number_out; i++) {

        out_port = "port_" + to_string(i);
        GWNOutPort port_new = GWNOutPort(this, out_port, i);
        if (debug) {
          std::cout << "... port_new.block->name: " << 
            port_new.block->name << std::endl;
        }
        ports_out[i] = &port_new;

        // from Python gwnblock
        //pmt_out_port = pmt::intern(out_port);

        // from message_strobe
        message_port_register_out(pmt::mp(out_port) ); 


        if (debug) {
          ports_out[i]->__str__();
          //std::cout << "to_string message: " << message << std::endl;
        }
      }
    }

    /* gwnblock: our virtual destructor */
    gwnblock_impl::~gwnblock_impl()
    {
    }


    std::string gwnblock_impl::__str__() {
      std::string ss = "__str__() Block name: " + this->name; 
      return ss;
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
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


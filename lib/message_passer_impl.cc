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

namespace gr {
  namespace gwncppvgb {

    message_passer::sptr
    message_passer::make(int counter, std::string message)
    {
      return gnuradio::get_initial_sptr
        (new message_passer_impl(counter, message));
    }

    /*
     * The private constructor
     */
    message_passer_impl::message_passer_impl(int counter, std::string message)
        : gwncppvgb::gwnblock_impl("message_passer", 1 , 2, 0, 0)
//      : gr::block("message_passer",
//              gr::io_signature::make(<+MIN_IN+>, <+MAX_IN+>, sizeof(<+ITYPE+>)),
//              gr::io_signature::make(<+MIN_OUT+>, <+MAX_OUT+>, sizeof(<+OTYPE+>)))
    {
      std::cout << "message_passer, constructor" << std::endl;
      std::cout << "   block name: " << name << std::endl;
      std::cout << "   number_out: " << number_out << std::endl;
      //std::cout << ports_out[0]->__str__() << std::endl; // does not show!
      
      int i;
      for ( i=0; i < number_out; i++) {
          //std::cout << ports_out[i]->__str__() << std::endl; 
          std::cout << "  out port " << i << 
            ", port: " << ports_out[i]->port << 
            ", port nr: " << ports_out[i]->port_nr << std::endl; 
          std::string dbg_msg = "message passer, post Message debug\n...";
          dbg_msg += ports_out[i]->__str__();
          std::cout << dbg_msg << std::endl;
      }


    }

    /*
     * Our virtual destructor.
     */
    message_passer_impl::~message_passer_impl()
    {
    }


    void
    message_passer_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
      // <+forecast+> e.g. ninput_items_required[0] = noutput_items
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
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


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
#include "gwnblock_impl.h"



namespace gr {
  namespace gwncppvgb {

    message_passer::sptr
    message_passer::make(int number, std::string message)
    {
      return gnuradio::get_initial_sptr
        (new message_passer_impl(number, message));
    }

    /*
     * The private constructor
     */
    message_passer_impl::message_passer_impl(int number, std::string message) : 
      gwnblock_impl::gwnblock_impl("message_passer", 1, 1, 0, 0)
    //  : gr::sync_block("message_passer",
    //          gr::io_signature::make(<+MIN_IN+>, <+MAX_IN+>, sizeof(<+ITYPE+>)),
    //          gr::io_signature::make(<+MIN_OUT+>, <+MAX_OUT+>, sizeof(<+OTYPE+>)))
    {
    std::cout << "message_passer, constructor" << std::endl;
    }

    /*
     * Our virtual destructor.
     */
    message_passer_impl::~message_passer_impl()
    {
    }

    int
    message_passer_impl::work(int noutput_items,
        gr_vector_const_void_star &input_items,
        gr_vector_void_star &output_items)
    {
      const int *in = (const int *) input_items[0];
      int *out = (int *) output_items[0];

      // Do <+signal processing+>

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace gwncppvgb */
} /* namespace gr */


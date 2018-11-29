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
#include <gwncppvgb/message_repeat_pdata.h>

/* GWN inclusions */
#include <stdio.h>
#include <iostream>



#include "message_repeat_impl.h"

namespace gr {
  namespace gwncppvgb {

    /*
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */


    /* GWN process_data function, rewrite for a new block */
    pmt::pmt_t
    message_repeat_pdata::process_data(
      //message_repeat_impl * p_block, std::string p_port,
      std::string p_port,
      pmt::pmt_t p_pmt_msg, int p_counter)
    {
      //d_block = p_block;
      d_port = p_port;
      d_pmt_msg = p_pmt_msg;
      d_counter = p_counter;
      std::cout << "...process_data, received message from port: " 
        << d_port << ", counter: " << d_counter << std::endl;
      std::cout << "...process_data, message: "; pmt::print(d_pmt_msg);
      //  << std::endl;

      // emit message 
      pmt::pmt_t pmt_port = pmt::string_to_symbol("out_port_0");
      //pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev_proc); 
      return pmt::cons(pmt_port, d_pmt_msg);
    }


    //message_repeat_pdata::message_repeat_pdata(std::string message, int counter)
    message_repeat_pdata::message_repeat_pdata(std::string message, int counter)
    {

      // GWN user arguments initialization

    }
    message_repeat_pdata::~message_repeat_pdata() { }

  } /* namespace gwncppvgb */
} /* namespace gr */


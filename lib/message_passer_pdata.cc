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
#include <gwncppvgb/message_passer_pdata.h>

/* GWN inclusions */
#include <stdio.h>
#include <iostream>
#include "message_passer_impl.h"

namespace gr {
  namespace gwncppvgb {

    /* GWN process_data function, rewrite for a new block */

    pmt::pmt_t message_passer_pdata::process_data(std::string ev)
    {
      std::string ev_proc = "...process_data, received: " +
        ev + "\n   User parameter message: " + d_message + 
        "\n   User parameter counter: " + to_string(d_counter) +
        "\n"; 
      d_counter = d_counter + 1;
      pmt::pmt_t pmt_port = pmt::string_to_symbol("out_port_0");
      pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev_proc); 
      return pmt::cons(pmt_port, pmt_msg);
    }


    /* GWN process_data class, leave as it is in a new block */
    message_passer_pdata::message_passer_pdata(std::string message, int counter)
    {
      // GWN user arguments initialization
      d_message = message;
      d_counter = counter;
    }
    message_passer_pdata::~message_passer_pdata() { }

  } /* namespace gwncppvgb */
} /* namespace gr */


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
#include <gwncppvgb/gwnblockc_pdata.h>

/* GWN inclusions */
#include <stdio.h>
#include <iostream>
#include "gwnblockc_impl.h"

namespace gr {
  namespace gwncppvgb {

    /* GWN process_data function, rewrite for a new block */

    std::string gwnblockc_pdata::process_data(std::string ev)
    {
    std::cout << "...gwnblockc_data, event received: " <<
      ev << std::endl;
    std::string ev_proc = "...gwnblockc, event processed: " + ev;
    return ev_proc;
    }


    /* GWN process_data class, leave as it is in a new block */
    gwnblockc_pdata::gwnblockc_pdata(std::string ev) { }
    gwnblockc_pdata::~gwnblockc_pdata() { }

  } /* namespace gwncppvgb */
} /* namespace gr */


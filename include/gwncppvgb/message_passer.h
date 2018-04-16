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


#ifndef INCLUDED_GWNCPPVGB_MESSAGE_PASSER_H
#define INCLUDED_GWNCPPVGB_MESSAGE_PASSER_H

#include <gwncppvgb/api.h>
#include <gnuradio/sync_block.h>

#include "../../lib/gwnblock_impl.h"


namespace gr {
  namespace gwncppvgb {

    /*!
     * \brief <+description of block+>
     * \ingroup gwncppvgb
     *
     */
    class GWNCPPVGB_API message_passer //: 
      //virtual public gwnblock_impl //, virtual public gr::block
    //class GWNCPPVGB_API message_passer : virtual public gr::sync_block
    {
     public:
      typedef boost::shared_ptr<message_passer> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gwncppvgb::message_passer.
       *
       * To avoid accidental use of raw pointers, gwncppvgb::message_passer's
       * constructor is in a private implementation
       * class. gwncppvgb::message_passer::make is the public interface for
       * creating new instances.
       */
      static sptr make(int number, std::string message);
    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_MESSAGE_PASSER_H */


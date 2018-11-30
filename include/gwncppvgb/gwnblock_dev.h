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


#ifndef INCLUDED_GWNCPPVGB_GWNBLOCK_DEV_H
#define INCLUDED_GWNCPPVGB_GWNBLOCK_DEV_H

#include <gwncppvgb/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace gwncppvgb {

    /*!
     * \brief GWN template class for new block construction. 
     * \ingroup gwncppvgb
     *
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */


    class GWNCPPVGB_API gwnblock_dev : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<gwnblock_dev> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of gwncppvgb::gwnblock_dev.
       *
       * To avoid accidental use of raw pointers, gwncppvgb::gwnblock_dev's
       * constructor is in a private implementation
       * class. gwncppvgb::gwnblock_dev::make is the public interface for
       * creating new instances.
       */
      static sptr make(std::string message, int counter);

    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_GWNBLOCK_DEV_H */


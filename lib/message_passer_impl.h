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

#ifndef INCLUDED_GWNCPPVGB_MESSAGE_PASSER_IMPL_H
#define INCLUDED_GWNCPPVGB_MESSAGE_PASSER_IMPL_H

#include <gwncppvgb/message_passer.h>
#include "gwnblock_impl.h"

namespace gr {
  namespace gwncppvgb {

    class message_passer_impl : public message_passer, public gwnblock_impl
    {
     private:
      // Nothing to declare in this block.

     public:
       message_passer_impl(int number, std::string message);

      //message_passer_impl(int, std::string) :
      //  gwnblock_impl("message_passer", 1, 1, 0, 0);
      //error: expected ‘{’ at end of input
      //   gwnblock_impl("message_passer", 1, 1, 0, 0);


      //message_passer_impl(int, std::string) :
      //  gwnblock_impl (std::string, int, int, int, int);
      //error: expected primary-expression before ‘,’ token
      //   gwnblock_impl (std::string, int, int, int, int);


      //message_passer_impl(int, std::string) :
      //  gwnblock_impl::gwnblock_impl (std::string, int, int, int, int);
      //error: expected primary-expression before ‘,’ token
      //   gwnblock_impl::gwnblock_impl (std::string, int, int, int, int);



      ~message_passer_impl();

      // Where all the action really happens
      int work(int noutput_items,
         gr_vector_const_void_star &input_items,
         gr_vector_void_star &output_items);
    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_MESSAGE_PASSER_IMPL_H */


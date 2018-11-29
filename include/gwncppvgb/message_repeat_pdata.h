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


#ifndef INCLUDED_GWNCPPVGB_MESSAGE_REPEAT_PDATA_H
#define INCLUDED_GWNCPPVGB_MESSAGE_REPEAT_PDATA_H

#include <gwncppvgb/api.h>

#include <string>
#include <pmt/pmt.h>

//#include "gwncppvgb/message_repeat.h"


namespace gr {
  namespace gwncppvgb {

  /*!
   * \brief GWN template class for new block construction
   *
   * Please consider some comments // and <text thus enclosed>
   * will be substituted by the gwn_modtool.sh script when
   * creating a new type of block. DO NOT ALTER, the script
   * needs those comments for correct substitution.
   */


    /** A class to contain a process data function.

    This class is instantiated into an object associated with the main block. The programmer is expected to code its own actions in the process_data function included in this class.
    */
    class GWNCPPVGB_API message_repeat_pdata
    {
    public:
      message_repeat_pdata(std::string message, int counter);
      ~message_repeat_pdata();


      /** Actions to perform on received message.

      This function receives messages from the input ports and from the timer ports. Origin of messages, i.e. specific input port or timer port, can be recognized in the port parameter. An optional counter can be included, generally used to indicate the sequence number of the message received. 

      @param p_port The port identifier on which the message was received.
      @param p_pmt_msg The message, in PMT format.
      @param p_counter An optional integer number, for individual message identifier through sequence numbers.
      @return A PMT tuple of (port, message) for the main block to emit.
      */
      //void process_data(message_repeat_impl * p_block,
      pmt::pmt_t process_data(
        std::string p_port, pmt::pmt_t p_pmt_msg, int p_counter);

    private:
      // GWN user arguments declaration
      std::string d_port;
      pmt::pmt_t d_pmt_msg;
      int d_counter;
    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_MESSAGE_REPEAT_PDATA_H */


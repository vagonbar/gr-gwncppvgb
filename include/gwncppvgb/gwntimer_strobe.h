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



#ifndef INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_H
#define INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_H

#include <gwncppvgb/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace gwncppvgb {

    /*!
     \brief Emits two series of messages based on two timers.
     \ingroup gwncppvgb
     
     A proof of concept to implement timers in GNU Radio. The
     block defines two timers instantiating a private class. Each timer
     emits a number of message periodically. Each timer emits its own
     message, has its own period and its own number of repeats.
     */
    class GWNCPPVGB_API gwntimer_strobe : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<gwntimer_strobe> sptr;

      /*!
       * \brief Public constructor.
       */
      static sptr make(
        std::string msg_1, float period_1, int count_1,
        std::string msg_2, float period_2, int count_2 );
      
      // failed attempt to make functions visble in QA
      //virtual void timer_reset(int timer_id);
      //virtual void timer_interrupt(int timer_id, bool on_off);

    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_H */


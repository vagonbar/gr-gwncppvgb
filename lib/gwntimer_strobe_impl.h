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

#ifndef INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_IMPL_H
#define INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_IMPL_H

#include <gwncppvgb/gwntimer_strobe.h>

#include <boost/thread.hpp>


namespace gr {
  namespace gwncppvgb {


    class gwntimer_strobe_impl : public gwntimer_strobe
    {
     private:

      /** GWN Timer, a timer with message, period, and count
       *
       * A nested class implementing a timer which can be instantiated
       * in the main block to receive messages periodically for a number
       * of times.
       */
      class GWNTimer
      {
        protected:
          bool d_debug;
        public:
          GWNTimer(gwntimer_strobe_impl * p_block, std::string p_msg, 
            int p_counter, float p_period_ms);

          gwntimer_strobe_impl * d_block;
          std::string d_msg;
          int d_counter;
          int d_count;
          float d_period_ms;

          boost::shared_ptr<gr::thread::thread> d_thread;
          bool d_finished;

          void set_finished(bool val)  { d_finished = val; }
          void run_timer();

          //std::string d_port;
          //int d_port_nr;
          //const pmt::pmt_t d_port;
          //std::string __str__();
      }; 


     public:
      gwntimer_strobe_impl();
      ~gwntimer_strobe_impl();

      pmt::pmt_t d_pmt_msg;
      pmt::pmt_t d_out_port;
      pmt::pmt_t d_timer_port;

      //boost::mutex d_mutex;

      void handle_msg (pmt::pmt_t pmt_msg);
      void post_timer_msg(std::string msg);
      
      bool start_timer();
      void process_data(std::string);

      /** Mutually exclusive printing.

      @param msg string to print.
      */
      void mutex_prt(std::string msg);

    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_IMPL_H */


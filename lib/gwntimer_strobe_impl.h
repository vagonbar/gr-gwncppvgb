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
       
      A nested class implementing a timer which can be instantiated
      in a GWN block to receive internal messages periodically 
      for a number of times.
 
      The block containing the timer receives in an internal port messages from the timer. This internal port is a message port of the block, but it is not no connected to any external block. This internal port is called timer_port, and all timers in the block send their messages to this unique timer port.

      When started, the timer waits for the indicated period of time before emitting its first message. Then, the timer goes on sending messages each period of time, until reaching the number indicated by a counter passed as a parameter. Once the counter has been reached, no more messages are sent, and the timer thread is terminated.

      The timer can be suspended in its emission of messages. When suspended, the timer will cease to emit messages, and the counter is not incremented, but the timer thread remains alive. When taken out from suspension, messages continue to be emitted and the counter incremented from its last value.

      The counter can be resetted, thus starting to emit messages as if it was recentrly started, for the number of timer originally indicated.
 
      The timer can be stopped before reaching its assigned number of messages to emit. In this case, no messages are emitted any more, and the timer thread is terminated.
      */
      class GWNTimer
      {
        protected:
          bool d_debug;

        public:

          /** \brief GWNTimer, constructor

          @param p_block A pointer to the block having the timer.
          @param p_msg The message to emit periodically.
          @param p_count The number of times to emit the message.
          @param p_period: Tne period of emission in milliseconds.
          */
          GWNTimer(gwntimer_strobe_impl * p_block, 
            std::string p_msg, 
            int p_counter, float p_period_ms);

          /** A pointer to the block containing the timer. */ 
          gwntimer_strobe_impl * d_block;
          std::string d_msg;
          int d_counter;
          int d_count;  /** Counts emitted messages. */
          float d_period_ms;

          /** The timer thread. */
          boost::shared_ptr<gr::thread::thread> d_thread;
          bool d_suspend;

          /** Suspends or continues emission of messages. */
          void set_suspend(bool on_off)  { d_suspend = on_off; }
          /** Sets or resets the counter, to 0 or other value. */
          void set_count(int counter)  { d_counter = counter; }

          /** \brief The function to run from the timer thread. */
          void run_timer();

      };   // end GWNTimer


     public:
      gwntimer_strobe_impl (
        std::string msg_1, float period_1, int count_1,
        std::string msg_2, float period_2, int count_2 );
      ~gwntimer_strobe_impl();

      // two timers
      GWNTimer * tm_1;
      GWNTimer * tm_2;

      //pmt::pmt_t d_pmt_msg;
      pmt::pmt_t d_out_port;
      pmt::pmt_t d_timer_port;


      void handle_msg (pmt::pmt_t pmt_msg);
      void post_timer_msg(std::string msg);
      
      bool start_timer();
      void process_data(std::string);

      /** Mutually exclusive printing.

      @param msg string to print.
      */
      //boost::mutex d_mutex;
      void mutex_prt(std::string msg);

      // failed attempt to make functions visble in QA
      void timer_reset(int timer_id);
      void timer_interrupt(int timer_id, bool on_off);


    private:
      std::string d_msg_1, d_msg_2;
      float d_period_1, d_period_2;
      int d_count_1, d_count_2;
 
    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_GWNTIMER_STROBE_IMPL_H */


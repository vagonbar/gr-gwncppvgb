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
    // private:  // works well, and desirable
    public:     // just to make visible Doxygen documentation

      /** GWN Timer, a timer with message, period, and count.
       
      A nested class implementing a timer which can be instantiated
      in a GWN block to receive internal messages periodically 
      for a number of times.
 
      The block containing the timer receives messages from the timer in an internal port. This internal port, called a timer port, is a message port of the block, but it is not no connected to any external block. All timers in the block send their messages to this unique timer port.

      When started, the timer waits for the indicated period of time before emitting its first message. Then, the timer goes on sending messages after each period of time, until reaching the number of messages indicated as a count parameter. Once the count has been reached, no more messages are sent, and the timer thread is terminated.

      The timer can be suspended in its emission of messages. When suspended, the timer does not emit messages, and the counter is not incremented, but the timer thread remains alive. When taken out from suspension, messages continue to be emitted and the counter incremented from its last value.

      The counter can be resetted, thus starting to emit messages as if it was recently started.
 
      The timer can be stopped before reaching its assigned number of messages to emit. In this case, no messages are emitted any more, and the timer thread is terminated.

      The message emmited by the timer is a tuple containing the type of emitting port as "timer_port", an integer indicating which timer port is the emitter for cases where a block contains several timers, a counter indicating the sequence number of message emitted, and a custom message defined by the user, in PMT format, which allows a great flexibility for the content to be transmitted. Each message is passed to the main block function process_data, where the tasks defined by the programmer of the block to which the timer is attached are performed. In this way, the programmer of the process_data function can identify unambiguously the message origin (a timer), the particular timer whch emitted the message, the sequence number of the message, and a possibly complex structure of data in the custom PMT message.
      */
      class GWNTimer
      {
        protected:
          bool d_debug;

        public:

          /** \brief GWNTimer, constructor.

          @param p_block A pointer to the block having the timer.
          @param p_id_timer An integer identifying the timer.
          @param p_pmt_msg Custom message to emit, in PMT format.
          @param p_count The number of times to emit the message.
          @param p_period: Tne period of emission in milliseconds.
          */
          GWNTimer(gwntimer_strobe_impl * p_block, 
            int p_id_timer, pmt::pmt_t p_pmt_msg, 
            int p_counter, float p_period_ms);

            gwntimer_strobe_impl * d_block;
            int d_id_timer; 
            pmt::pmt_t d_pmt_msg;
            int d_counter;
            int d_count;  /** Counts emitted messages. */
            float d_period_ms;
            bool d_suspend; /** If true timer is suspendend. */

            /** The timer thread. */
            boost::shared_ptr<gr::thread::thread> d_thread;

            /** Suspends or continues emission of messages. */
            void set_suspend(bool on_off)  { d_suspend = on_off; }
            /** Sets or resets the counter, to 0 or other value. */
            void set_count(int counter)  { d_counter = counter; }
            /** Stops timer and terminates thread. */
            void timer_stop();

            /** \brief The function to run from the timer thread. */
            void run_timer();

      };   // end GWNTimer


     //public:   // if GWNTimer is public
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


      void handle_timer_msg (pmt::pmt_t pmt_msg);
      void post_timer_msg(std::string msg);
      
      bool start_timer();
      void process_data(pmt::pmt_t pmt_msg);

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


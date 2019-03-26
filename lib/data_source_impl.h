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

#ifndef INCLUDED_GWNCPPVGB_DATA_SOURCE_IMPL_H
#define INCLUDED_GWNCPPVGB_DATA_SOURCE_IMPL_H

#include <gwncppvgb/data_source.h>

/*  GWN inclusions */
#include <vector>

// GWN TAG include FSM block

namespace gr {
  namespace gwncppvgb {

    /*
     * Please consider some comments // and <text thus enclosed>
     * will be substituted by the gwn_modtool.sh script when
     * creating a new type of block. DO NOT ALTER, the script
     * needs those comments for correct substitution.
     */


    /* GWN data_source, a template block */
    class data_source_impl : public virtual data_source
    {
    private:

      /** GWN port, a generic class for input and output messages. */
      class GWNPort
      {
        protected:
          bool d_debug;
        public:
          GWNPort();
          /** A pointer to the block which contains this port. */ 
          data_source_impl * d_block;
          /** An identifier of this port. */
          std::string d_port;
          /** A sequential number for this port. */ 
          int d_port_nr;
          /** Prints message port information. */
          std::string __str__();
      }; 

      /** Output message port.
      */
      class GWNOutPort: public virtual GWNPort { 
        public:
          GWNOutPort(data_source_impl *, std::string, int);
       };

      /** Input message port.
      */
      class GWNInPort: public virtual GWNPort {
        public:
          GWNInPort(data_source_impl *, std::string, int);
      }; 



      /* GWN timers, nested class */


      /** GWN Timer, a timer with message, period, and count.
       
      A nested class implementing a timer which can be instantiated
      into an object and associated to a GWN block. The timer emits
      internal messages periodically for a number of times.
 
      The block containing the timer receives messages from the timer in an internal port. This internal port, called a timer port, is a message port of the block, but it is not no connected to any external block. All timers in the block send their messages to this unique timer port.

      When started, the timer waits for the indicated period of time before emitting its first message. Then, the timer goes on sending messages after each period of time, until reaching the number of messages indicated as a count parameter. Once the count has been reached, no more messages are sent. However, the timer thread is not finished, and can be restarted.

      The timer can be suspended in its emission of messages. When suspended, the timer does not emit messages, and the counter is not incremented, but the timer thread remains alive. When taken out from suspension, messages continue to be emitted and the counter is incremented from its last value.

      The counter can be resetted, thus starting to emit messages as if it was recently started.
 
      The timer can be stopped before reaching its assigned number of messages to emit. In this case, no messages are emitted any more, and the timer thread is terminated.

      The message emmited by the timer is a GWN message, i.e. a port identifier and a dictionary in PMT format. In a GWN message, the dictionary contains a type, a subtype, and a sequence number, with the optional addition of other entries defined by the user. Each message is passed to the main block function process_data, where the actions defined by the programmer happen.
      */
      class GWNTimer
      {
        protected:
          bool d_debug;

        public:

          /** \brief GWNTimer, constructor.

          @param block A pointer to the block having the timer.
          @param id_timer An integer identifying the timer.
          @param pmt_msg Custom message to emit, in PMT format.
          @param count The number of times to emit the message.
          @param period_ms: Tne period of emission in milliseconds.
          */
          GWNTimer(data_source_impl * block, 
            std::string id_timer, pmt::pmt_t pmt_msg, 
            int count, float period_ms);

            data_source_impl * d_block;
            std::string d_id_timer; 
            pmt::pmt_t d_pmt_msg;
            int d_count;
            float d_period_ms;

            /** Counts emitted messages. */
            int d_counter;
            /** If true timer is suspendend. */
            bool d_suspend;
            /** a port to receive messages from all timers. */
            pmt::pmt_t d_pmt_timer_port = pmt::mp("timer_port");
            /** The timer thread. */
            boost::shared_ptr<gr::thread::thread> d_thread;
            /** Creates thread, timer starts immediately. */
            void start_timer();
            /** Suspends or continues emission of messages. */
            void set_suspend(bool on_off)  { d_suspend = on_off; }
            /** Sets or resets the counter, to 0 or other value. */
            void set_count(int counter)  { d_counter = counter; }
            /** Stops timer and terminates thread. */
            void timer_stop();

            /** The function to run from the timer thread. */
            void run_timer();

      };   // end class GWNTimer

      /** Handles timer messages.

      @param pmt_msg: a message in PMT format.
      */

      void handle_timer_msg (pmt::pmt_t pmt_msg);

      // GWN TAG: user arguments declaration
      //std::string d_message;
      //int d_counter;


    public:
      /** Constructor. */
      data_source_impl( std::string type, std::string subtype, std::string payload, float period, int count );

      /** Destructor. */
      ~data_source_impl();

      /** Block name. */
      std::string d_name;
      /** Number of input ports. */
      int d_number_in;
      /** Number of output ports. */
      int d_number_out;
      /** Number of timer ports. */
      int d_number_timers;
      /** A vector (array) of output ports. */
      std::vector<GWNOutPort *> d_ports_out;
      /** A vector (array) of input ports. */
      std::vector<GWNInPort *> d_ports_in;
      /** A vector (array) of timer ports. */
      std::vector<GWNTimer *> d_timers;

      /** Posts message on PMT formatted output port. 

      @param pmt_port: the port identifier, in PMT format.
      @param pmt_msg: the message in PMT format, any PMT type.
      */
      void post_message(pmt::pmt_t pmt_port, pmt::pmt_t pmt_msg);
      /** Posts message on string formatted output port. 

      @param pmt_port: the port identifier, in string format.
      @param pmt_msg: the message in PMT format, any PMT type.
      */
      void post_message(std::string port, pmt::pmt_t pmt_msg);
      /** Handles messages received in an input port.

      @param pmt_msg: the message in PMT format, any PMT type.
      */
      void handle_msg(pmt::pmt_t pmt_msg);
      /** Prints block info. */
      std::string __str__();


      /** REWRITE: specific initialization for a new block.

      This function is invoked at the end of the block constructor to account for additional initialization required to perform the specific actions of the  new block, such as timers initialization. 
      */
      void added_init();


      /** REWRITE: actions to perform on received message.

      This function receives messages from the input ports and from the timer ports. 
      The message received may be any PMT formatted message. If it is a GWN message, it is a dictionary in PMT format, which contains a type, a subtype, and a sequence number, with the optional addition of other entries defined by the user. In this function the actions defined by the programmer are executed.
      @param port The port identifier on which the message was received.
      @param pmt_msg The message, in PMT format.
      */
      void process_data(
        std::string port, pmt::pmt_t pmt_msg);

    private:
      bool d_debug;

      // GWN TAG user arguments declaration
      std::string d_type;
      std::string d_subtype;
      std::string d_payload;
      float d_period;
      int d_count;

      int d_seq_nr;
      pmt::pmt_t pmt_msg_dict;
    }; 

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_DATA_SOURCE_IMPL_H */


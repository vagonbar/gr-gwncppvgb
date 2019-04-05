/* -*- c++ -*- */
/* 
 * Copyright 2019 <+YOU OR YOUR COMPANY+>.
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
#include <gwncppvgb/stop_wait_send_fsm.h>



namespace gr {
  namespace gwncppvgb {



    /* ********************************************** 
     * FSM specific code, REWRITE for a new FSM.
     ************************************************/

    // Stop and Wait actions

    void fn_send(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      //d_fsm.d_ack_waited = true;    // start waiting for ACK
      d_fsm.d_retries = 0;          // this msg not yet send
      d_fsm.d_command = "Transmit"; // to output message
      d_fsm.d_msg_to_send = d_fsm.d_msg_received;  // msg waiting ACK
    }
    void fn_ack_ok(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      //d_fsm.d_ack_waited = false;    // ACK received
      d_fsm.d_command = "AckReceived";  // message will not be output
    }
    void fn_sendfrombuffer(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      //d_fsm.d_ack_waited = true;    // start waiting for ACK
      d_fsm.d_retries = 0;          // this msg not yet send
      d_fsm.d_command = "Transmit"; // to output message
      d_fsm.d_msg_to_send = d_fsm.d_memory.back();  // oldest msg
      d_fsm.d_memory.pop_back();                    // delete oldest msg
    }
    void fn_resend(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      // d_ack_waited remains true
      d_fsm.d_command = "Transmit";          // to output message
      d_fsm.d_retries = d_fsm.d_retries + 1;   // increment retries
      // d_msg_to_send sent again
    }
    void fn_push(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      d_fsm.d_memory.push_front(d_fsm.d_msg_received);  // store msg
    }
    void fn_stop(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_memory.size() > d_fsm.d_buf_len) 
        d_fsm.d_command = "StopBufferFull"; // abort execution
      else if (d_fsm.d_retries > d_fsm.d_max_retries) 
        d_fsm.d_command = "StopNoRetriesLeft";
      else
        d_fsm.d_command = "StopOther";
    }


    // Stop and Wait conditions
    //bool ack_waited(gwncppvgb::stop_wait_send_fsm &d_fsm) {
    //  if (d_fsm.d_ack_waited) {
    //    return true;
    //  } else {
    //    return false;
    //  }
    //} 
    bool cnd_buffer_empty(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_memory.empty()) {
        return true;
      } else {
        return false;
      }
    }
    bool cnd_buffer_full(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_memory.size() > d_fsm.d_buf_len) {
        return true;
      } else {
        return false;
      }
    }
    bool cnd_not_buffer_full(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_memory.size() <= d_fsm.d_buf_len) {
        return true;
      } else {
        return false;
      }
    }
    bool cnd_retries_left(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_retries <= d_fsm.d_max_retries) {
        return true;
      } else {
        return false;
      }
    }
    bool cnd_no_retries_left(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if (d_fsm.d_retries > d_fsm.d_max_retries) {
        return true;
      } else {
        return false;
      }
    }
    bool cnd_msg_buffer(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      if ( ! d_fsm.d_memory.empty() ) {
       return true;
      } else {
        return false;
      }
    }
    //bool ack_waited_buffer_empty(gwncppvgb::stop_wait_send_fsm &d_fsm) {
    //  //if ( ack_waited(d_fsm) && buffer_empty(d_fsm) ) {
    //  if ( d_fsm.d_ack_waited && d_fsm.d_memory.empty() ) {
    //    return true;
    //  } else {
    //    return false;
    //  }
    //}


    // FSM generic action functions definitions
    void fn_none(gwncppvgb::stop_wait_send_fsm &d_fsm) {
        //d_fsm.d_action_result += "Result of function fn_none. ";
      return;
    }
    void fn_error(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      std::cout << "  --- FSM error " << std::endl;
      //d_fsm.d_action_result += "FSM ERROR";
      return;
    }
    void fn_init(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      //std::cout << "  --- FSM fn_init" << std::endl;
      //d_fsm.d_action_result += "Result of function fn_init. ";
      return;
    }


    // FSM generic condition function definitions
    bool cnd_true(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      return true;
    }
    bool cnd_false(gwncppvgb::stop_wait_send_fsm &d_fsm) {
      return false;
    }

    // other user defined functions */


    /* FSM transitions */
    void 
    stop_wait_send_fsm::add_myfsm_transitions()
    {
      // add ordinary transitions
      add_transition("Data", "Idle", fn_send, "WaitAck", 
        cnd_true, "Idle, Data, send message, to WaitAck"); 
      add_transition("Control", "WaitAck", fn_ack_ok, "Idle",
        cnd_buffer_empty, "WaitAck, ACK, buffer empty, to Idle"); 
      add_transition("Control", "WaitAck", fn_sendfrombuffer, "WaitAck",
        cnd_msg_buffer, "WaitAck, ACK, msg buffer, to WaitAck"); 
      add_transition("Timer", "WaitAck", fn_resend, "WaitAck",
        cnd_retries_left, "WaitAck, timeout, resend, to WaitAck"); 
      add_transition("Data", "WaitAck", fn_push, "WaitAck", 
        cnd_not_buffer_full, "WaitAck, Data, push buffer, to WaitAck"); 
      add_transition("Data", "WaitAck", fn_stop, "Stop", 
        cnd_buffer_full, "WaitAck, Data, buffer full, to Stop"); 
      add_transition("Timer", "WaitAck", fn_stop, "WaitAck", 
        cnd_no_retries_left, "Idle, timeout, send message, to WaitAck"); 

      // default transition
      add_transition ("", "", fn_error, "Idle", cnd_true, "default transition");

      // transitions for any input symbol
      //add_transition ("", "Idle", fn_none, "Idle", cnd_true,
      //    "any symbol");
    }


    /* FSM process function */
    pmt::pmt_t 
    stop_wait_send_fsm::process(std::string port, pmt::pmt_t pmt_msg)
    {
      std::string d_port = port;
      d_msg_received = pmt_msg;
      // unpack message, a PMT dictionary
      std::string type;
      std::string subtype;
      int seq_nr;
      // GWN message, unpack type, subtype, seq_nr, FSM symbol
      type = pmt::symbol_to_string (pmt::dict_ref(
        pmt_msg, pmt::intern("type"), pmt::PMT_NIL));
      subtype = pmt::symbol_to_string (pmt::dict_ref(
        pmt_msg, pmt::intern("subtype"), pmt::PMT_NIL));
      seq_nr = pmt::to_long (pmt::dict_ref(
        pmt_msg, pmt::intern("seq_nr"), pmt::PMT_NIL)); 

      d_input_symbol = type;
      
      // FSM actions
      from_state stt_search;

      // search for ordinary transitions
      stt_search = std::make_tuple(d_input_symbol, d_current_state);
      if ( exec_transition(stt_search) ) {
        pmt::pmt_t pmt_return_tuple = pmt::make_tuple( 
          pmt::mp(d_command), d_msg_to_send);
        return pmt_return_tuple;
      }

      // search for transitions for any input symbol
      stt_search = std::make_tuple("", d_current_state);
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, state) transition" << 
            d_input_symbol << ", " << d_current_state << ")\n";
         }
        d_command = "AnySymbolTransition";
        pmt::pmt_t pmt_return_tuple = pmt::make_tuple( 
          pmt::mp(d_command), d_msg_to_send);
        return pmt_return_tuple;
      }
      // search for default transition
      stt_search = std::make_tuple("", "");
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, any state) transition" <<
            d_input_symbol << ", " << d_current_state << ")\n";
        //return pmt::mp(d_action_result);
        // return 2-tuple, a command and message (diccionary)
        d_command = "DefaultTransition";
        pmt::pmt_t pmt_return_tuple = pmt::make_tuple( 
          pmt::mp(d_command), d_msg_to_send);
        return pmt_return_tuple;
        }
      }
      else
      {
        std::cout << "Default transition not found" << std::endl;
        throw ExceptionFSM("stop_wait_send_fsm EXCEPTION: no valid transitions!");
      }
    }

    /* ********************************************** 
     *  End FSM specific code.
     ************************************************/


    /* stop_wait_send_fsm exception functions */
    stop_wait_send_fsm::ExceptionFSM::ExceptionFSM(std::string value) 
      {
        d_value = value;
        std::cout << "ExceptionFSM, constructor" << std::endl;
      } 

    stop_wait_send_fsm::ExceptionFSM::~ExceptionFSM() {}

    std::string
    stop_wait_send_fsm::ExceptionFSM::get_value()
      {
        std::cout << "ExceptionFSM, get_value" << std::endl;
        return d_value;
      }


    /* Constructor */
    stop_wait_send_fsm::stop_wait_send_fsm(std::string initial_state,
      int max_retries, float timeout, int buf_len, bool fsm_debug) : 
      d_initial_state(initial_state), d_max_retries(max_retries), 
        d_buf_len(buf_len), d_fsm_debug(fsm_debug)
    {
      d_default_transition =  "";
      d_input_symbol = "";
      d_initial_state = initial_state;
      d_current_state = initial_state;
      d_next_state = "";
      d_action_result = pmt::get_PMT_NIL();
      add_myfsm_transitions();    // FSM add transitions
      //d_debug = true;
      d_debug = false;

    }


    /* Destructor */
    stop_wait_send_fsm::~stop_wait_send_fsm()
    {
    }


    void
    stop_wait_send_fsm::null_action(stop_wait_send_fsm fsm) {}

    bool
    stop_wait_send_fsm::null_condition(stop_wait_send_fsm fsm) {return true;}


    void
    stop_wait_send_fsm::reset()
    {
        d_current_state = d_initial_state;
        d_input_symbol = "";
    }


    void 
    stop_wait_send_fsm::add_transition(std::string input_symbol, std::string state,
        stop_wait_send_fsm::type_action fn_action, std::string next_state, 
        type_condition fn_cond, std::string comment)
    {
      from_state frstt = make_tuple(input_symbol, state); 
      to_state tostt = make_tuple(fn_action, next_state, fn_cond,
        comment);
      d_state_transitions.insert(make_pair(frstt, tostt));
    }


    std::string 
    stop_wait_send_fsm::get_state()
    {
      std::string msg ="...stop_wait_send_fsm state: input symbol: " + d_input_symbol +
        //", initial state " + d_initial_state + 
        ",  current state " + d_current_state + "\n";
      return msg;
    }


    std::string
    stop_wait_send_fsm::get_transition(from_state frstt, to_state tostt)
    {
      /*std::cout << "  (" <<
        std::get<0>(frstt) << ", " << std::get<1>(frstt) << 
          ") = (action, " << std::get<1>(tostt) <<
          ", condition, " << std::get<3>(tostt) <<
          ")" << std::endl;  */

      std::string msg = "  (" +
        std::get<0>(frstt) + ", " + std::get<1>(frstt) + 
          ") = (action, " + std::get<1>(tostt) +
          ", condition, " + std::get<3>(tostt) +
          ")\n"; 
      return msg;

        // TODO: how to print action and condition functions
        //"action function" << ", " << std::get<1>(frstt) << ") = (" <<
        // ", " << "condition function" << ")" << std::endl;
    }


    void
    stop_wait_send_fsm::print_transitions()
    {
      std::cout << "...stop_wait_send_fsm transitions:" << std::endl;
      std::map<from_state, to_state>::const_iterator it; 
      it = d_state_transitions.begin();
      while (it != d_state_transitions.end()) 
      {
        // print one transition
        from_state frstt = it->first;
        to_state tostt = it->second;
        std::cout << get_transition(frstt, tostt);
        ++it;
      } 
      std::cout << std::endl;
    } 


    void
    stop_wait_send_fsm::search_trans_print(from_state stt_search)
    {
      auto range_end = d_state_transitions.end();
      auto range = d_state_transitions.equal_range(stt_search);
      auto ptr = range.first;
      //auto ptr_end = range_end.first;
      if ( ptr == range_end ) 
        {
          //std::cout << "transitions not found.\n";
          return;
        }
      //else
      //  std::cout << "transitions found:\n";

      for (auto i = range.first; i != range.second; ++i)
        {
          from_state frstt = i->first;
          to_state tostt = i->second;
          std::cout << get_transition(frstt, tostt);
        }
    }

    bool
    stop_wait_send_fsm::exec_transition(from_state stt_search)
    {
      auto range_end = d_state_transitions.end();

      auto range = d_state_transitions.equal_range(stt_search);
      auto ptr = range.first;
      if ( ptr != range_end ) 
      {
        for (auto i = range.first; i != range.second; ++i)
        {
          from_state frstt = i->first;
          to_state tostt = i->second;

          // evalutate condition
          type_condition fn_cond = std::get<2>(tostt); 
          if ( fn_cond( *this ) )                 // eval condition
          {
            stop_wait_send_fsm::type_action fn_action = std::get<0>(tostt);
            fn_action( *this);  // execute action

            d_current_state = std::get<1>(tostt); // set to next state
            if (d_debug) {
              std::cout << "  Executed transition: "; 
              std::cout << get_transition(frstt, tostt);
              //std::cout << get_state();
            }
            return true;
          }  // end evaluate condition
        }  // end for
        return false;
      }
      return false;
    }


    void
    stop_wait_send_fsm::set_debug(bool dbg)
    {
      d_debug = dbg;
    }


  } /* namespace gwncppvgb */
} /* namespace gr */


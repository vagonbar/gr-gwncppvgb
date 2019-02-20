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
#include <gwncppvgb/fsmblk.h>



namespace gr {
  namespace gwncppvgb {



    /* ********************************************** 
     * FSM specific code, REWRITE for a new FSM.
     ************************************************/


    // action functions definitions

    void fn_goA(gwncppvgb::fsmblk &d_fsm) {
      //fsm.where = "B";
      //fsm.set_where("B");
      d_fsm.mem_push("A visited!");
      d_fsm.d_action_result = "Result of function fn_goA";
      std::cout << "  --- FSM fn_goA" << d_fsm.where << std::endl;
      return;
    }

    void fn_error(gwncppvgb::fsmblk &d_fsm) {
      std::cout << "  --- FSM error " << std::endl;
      d_fsm.d_action_result="FSM ERROR";
    }
    void fn_init(gwncppvgb::fsmblk &d_fsm) {
      std::cout << "  --- FSM fn_init" << std::endl;
      d_fsm.d_action_result = "Result of function fn_init";
      return;
    }


    // condition function definitions

    bool cnd_A(gwncppvgb::fsmblk &d_fsm) {
      return d_fsm.where == "A";
    }
    bool cnd_true(gwncppvgb::fsmblk &d_fsm) {
      return true;
    }


    // other user defined functions */

    void
    fsmblk::set_where(std::string value)
    {
      where = value;
    }
  

    // user transitions

    //void fsmblk::add_myfsm_transitions(gwncppvgb::fsmblk &d_fsm)
    void fsmblk::add_myfsm_transitions()
    {
      add_transition ("g", "INIT", fn_goA, "STATE_A", cnd_A, "where==A");
      add_transition ("r", "STATE_A", fn_init, "INIT", cnd_true);
      // default transition
      add_transition ("", "", fn_error, "INIT", cnd_true, "default transition");
      
    }


    /* ********************************************** 
     *  End FSM specific code.
     ************************************************/



    /* fsmblk exception functions */
/*    ExceptionFSM::ExceptionFSM(std::string value) 
      {
        d_value = value;
        std::cout << "ExceptionFSM, constructor" << std::endl;
      } 

    ExceptionFSM::~ExceptionFSM() {}

    std::string
    ExceptionFSM::get_value()
      {
        std::cout << "ExceptionFSM, get_value" << std::endl;
        return d_value;
      }
*/

    /* Constructor */
    fsmblk::fsmblk(std::string initial_state)
    {
      d_default_transition =  "";
      d_input_symbol = "";
      d_initial_state = initial_state;
      d_current_state = initial_state;
      d_next_state = "";
      d_action_result = "No action result";
      add_myfsm_transitions();    // FSM add transitions
      d_debug = true;

    }


    /* Destructor */
    fsmblk::~fsmblk()
    {
    }




    void
    fsmblk::null_action(fsmblk fsm) {}

    bool
    fsmblk::null_condition(fsmblk fsm) {return true;}


    void
    fsmblk::reset()
    {
        d_current_state = d_initial_state;
        d_input_symbol = "";
    }


    void 
    fsmblk::add_transition(std::string input_symbol, std::string state,
        type_action fn_action, std::string next_state, 
        type_condition fn_cond, std::string comment)
    {
      from_state frstt = make_tuple(input_symbol, state); 
      to_state tostt = make_tuple(fn_action, next_state, fn_cond,
        comment);
      d_state_transitions.insert(make_pair(frstt, tostt));
    }


    void
    fsmblk::print_state()
    {
      std::cout << "  fsmblk state: input symbol: " << d_input_symbol <<
        ", initial state " << d_initial_state << 
        ",  current state " << d_current_state << std::endl;
    }


    void fsmblk::print_transition(from_state frstt, to_state tostt)
    {
      std::cout << "  (" <<
        std::get<0>(frstt) << ", " << std::get<1>(frstt) << 
          ") = (action, " << std::get<1>(tostt) <<
          ", condition, " << std::get<3>(tostt) <<
          ")" << std::endl;
        // TODO: how to print action and condition functions
        //"action function" << ", " << std::get<1>(frstt) << ") = (" <<
        // ", " << "condition function" << ")" << std::endl;
    }


    void
    fsmblk::print_transitions()
    {
      std::cout << "fsmblk transitions:" << std::endl;
      std::map<from_state, to_state>::const_iterator it; 
      it = d_state_transitions.begin();
      while (it != d_state_transitions.end()) 
      {
        // print one transition
        from_state frstt = it->first;
        to_state tostt = it->second;
        print_transition(frstt, tostt);
        ++it;
      } 
      std::cout << std::endl;
    } 


    void
    fsmblk::search_trans_print(from_state stt_search)
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
          print_transition(frstt, tostt);
        }
    }


    /** Receives symbol, executes transition, moves machine.
     *
     * Receives a symbol, looks for a transition valid for the current state, verifies condition, executes action and returns result.
     *  @param input_symbol the received symbol.
     * @param message a string message.
     * @param block a pointer to the main block to which the FSM is attached.
     * @return FSM transition action result.
     */
    std::string
    fsmblk::process(std::string input_symbol, std::string message,
      std::string block)
    {
      d_input_symbol = input_symbol;  // only for printing
      from_state stt_search;

      // search for ordinary transitions
      stt_search = std::make_tuple(input_symbol, d_current_state);
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (symbol, state) transition" << std::endl;
        }
      return d_action_result;
      }

      // search for transitions for any input symbol
      stt_search = std::make_tuple("", d_current_state);
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, state) transition" << std::endl;
         }
        return d_action_result;
      }
      // search for default transition
      stt_search = std::make_tuple("", "");
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, any state) transition" << std::endl;
        return d_action_result;
        }
      }
      else
      {
        std::cout << "Default transition not found" << std::endl;
        //throw ExceptionFSM("fsmblk EXCEPTION: no valid transitions!");
      }
    }


    bool
    fsmblk::exec_transition(from_state stt_search)
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
            //std::cout << "exec debug, where=" << where <<
            //  " this->where=" << this->where << std::endl;
            type_action fn_action = std::get<0>(tostt);
            fn_action( *this);  // execute action

            d_current_state = std::get<1>(tostt); // set to next state
            if (d_debug) {
              std::cout << "  Executed transition: "; 
              print_transition(frstt, tostt);
              print_state();
            }
            return true;
          }
        }
        return false;
      }
    }


    void
    fsmblk::set_debug(bool dbg)
    {
      d_debug = dbg;
    }



    /* fsmblk memory */

    void
    fsmblk::mem_push(mem_type obj) {
      d_memory.push(obj); 
    }

    void
    fsmblk::mem_pop() {
      d_memory.pop(); 
    } 

    bool
    fsmblk::mem_empty() {
      return d_memory.empty();
    }

    int
    fsmblk::mem_size() {
      return d_memory.size();
    }

    mem_type
    fsmblk::mem_top() {
      return d_memory.top();
    }



  } /* namespace gwncppvgb */
} /* namespace gr */


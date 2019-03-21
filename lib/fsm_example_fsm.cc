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
#include <gwncppvgb/fsm_example_fsm.h>



namespace gr {
  namespace gwncppvgb {



    /* ********************************************** 
     * FSM specific code, REWRITE for a new FSM.
     ************************************************/


    // action functions definitions
    void fn_none(gwncppvgb::fsm_example_fsm &d_fsm) {
        d_fsm.d_action_result += "Result of function fn_none. ";
      return;
    }

    void fn_error(gwncppvgb::fsm_example_fsm &d_fsm) {
      //std::cout << "  --- FSM error " << std::endl;
      d_fsm.d_action_result += "FSM ERROR";
    }
    void fn_init(gwncppvgb::fsm_example_fsm &d_fsm) {
      //std::cout << "  --- FSM fn_init" << std::endl;
      d_fsm.d_action_result += "Result of function fn_init. ";
      return;
    }


    void fn_show(gwncppvgb::fsm_example_fsm &d_fsm) {
        std::string fsm_vars = "...FSM variables: where=" +
          d_fsm.where + ", to_c=" +
          std::to_string(d_fsm.to_c) + ". "; 
        std::cout << fsm_vars << std::endl;
        std::cout << "...FSM memory show and erase, INIT state" << std::endl;
        std::cout << "   Memory size: " << d_fsm.d_memory.size() <<
          ", contents:" << std::endl;
      while (!d_fsm.d_memory.empty())
      {
         std::cout << " : " << d_fsm.d_memory.front();
         d_fsm.d_memory.pop_front();
      }
      std::cout << std::endl;
        return;
    }


    void fn_goA(gwncppvgb::fsm_example_fsm &d_fsm) {
      //fsm.where = "B";
      //fsm.set_where("B");
      d_fsm.d_memory.push_front( pmt::mp("A visited!") );
      d_fsm.d_action_result += "Result of function fn_goA. ";
      //std::cout << "  --- FSM fn_goA" << d_fsm.where << std::endl;
      return;
    }

    void fn_goB(gwncppvgb::fsm_example_fsm &d_fsm) {
        //fsm.where = "C";
        d_fsm.d_memory.push_front( pmt::mp("B visited!") );
        d_fsm.d_action_result += "Result of function fn_goB. ";
        return;
    }

    void fn_goC(gwncppvgb::fsm_example_fsm &d_fsm) {
        //fsm.where = "A";
        d_fsm.d_memory.push_front( pmt::mp("C visited!") );
        d_fsm.d_action_result += "Result of function fn_goC; where=" + d_fsm.where + ". ";
        return;
    }

    void fn_goAB(gwncppvgb::fsm_example_fsm &d_fsm) {
      fn_goA(d_fsm);
      fn_goB(d_fsm);
      d_fsm.d_memory.push_front( pmt::mp("C visited! ") );
    }


    void fn_chgtoC(gwncppvgb::fsm_example_fsm &d_fsm) {
        if (d_fsm.to_c == true )
          d_fsm.to_c = false; 
        else
          d_fsm.to_c = true;
        d_fsm.d_action_result += "Result of fn_toC, to_c=" +
          std::to_string(d_fsm.to_c) + ". ";
        return;
    }

    void fn_chgwhr(gwncppvgb::fsm_example_fsm &d_fsm) {
      if (d_fsm.where == "A")
          d_fsm.where = "B";
      else if (d_fsm.where == "B")
          d_fsm.where = "C";
      else
          d_fsm.where = "A";
      d_fsm.d_action_result += "Result of fn_chgwhr, where=" + d_fsm.where + ". ";
      return;
   }



    // condition function definitions

    bool cnd_true(gwncppvgb::fsm_example_fsm &d_fsm) {
      return true;
    }
    bool cnd_false(gwncppvgb::fsm_example_fsm &d_fsm) {
      return false;
    }
    bool cnd_A(gwncppvgb::fsm_example_fsm &d_fsm) {
      return d_fsm.where == "A";
    }
    bool cnd_B(gwncppvgb::fsm_example_fsm &d_fsm) {
      return d_fsm.where == "B";
    }
    bool cnd_C(gwncppvgb::fsm_example_fsm &d_fsm) {
      return d_fsm.where == "C";
    }
    bool cnd_C_to_c(gwncppvgb::fsm_example_fsm &d_fsm) {
      return d_fsm.where == "C" && d_fsm.to_c == true;
    }


    // other user defined functions */

    void
    fsm_example_fsm::set_where(std::string value)
    {
      where = value;
    }
  

    // user transitions

    void 
    fsm_example_fsm::add_myfsm_transitions()
    {

      // default transition
      add_transition ("", "", fn_error, "INIT", cnd_true, "default transition");

      // transitions for any input symbol
      add_transition ("", "INIT", fn_none, "INIT", cnd_true,
          "any symbol");
      add_transition ("", "STATE_A", fn_none, "STATE_A", cnd_true,
          "any symbol");

      // add ordinary transitions
      add_transition ("s", "INIT", fn_show, "INIT", cnd_true);
      add_transition ("g", "INIT", fn_goA, "STATE_A", cnd_A, "where==A");
      add_transition ("g", "INIT", fn_goB, "STATE_B", cnd_B, "where==B");
      add_transition ("g", "INIT", fn_goAB, "STATE_C", cnd_C_to_c, "where==C && to_c" ); 
      add_transition ("r", "STATE_A", fn_init, "INIT", cnd_true);
      add_transition ("r", "STATE_B", fn_init, "INIT", cnd_true);
      add_transition ("r", "STATE_C", fn_init, "INIT", cnd_true);
      add_transition ("w", "INIT", fn_chgwhr, "CHG_WHERE", cnd_true);
      add_transition ("c", "INIT", fn_chgtoC, "CHG_TOC", cnd_true);
      add_transition ("r", "CHG_WHERE", fn_init, "INIT", cnd_true);
      add_transition ("r", "CHG_TOC", fn_init, "INIT", cnd_true);
      
    }


    /* ********************************************** 
     *  End FSM specific code.
     ************************************************/



    /* fsm_example_fsm exception functions */
    fsm_example_fsm::ExceptionFSM::ExceptionFSM(std::string value) 
      {
        d_value = value;
        std::cout << "ExceptionFSM, constructor" << std::endl;
      } 

    fsm_example_fsm::ExceptionFSM::~ExceptionFSM() {}

    std::string
    fsm_example_fsm::ExceptionFSM::get_value()
      {
        std::cout << "ExceptionFSM, get_value" << std::endl;
        return d_value;
      }


    /* Constructor */
    fsm_example_fsm::fsm_example_fsm(std::string initial_state)
    {
      d_default_transition =  "";
      d_input_symbol = "";
      d_initial_state = initial_state;
      d_current_state = initial_state;
      d_next_state = "";
      d_action_result = "";
      add_myfsm_transitions();    // FSM add transitions
      //d_debug = true;
      d_debug = false;

    }


    /* Destructor */
    fsm_example_fsm::~fsm_example_fsm()
    {
    }


    void
    fsm_example_fsm::null_action(fsm_example_fsm fsm) {}

    bool
    fsm_example_fsm::null_condition(fsm_example_fsm fsm) {return true;}


    void
    fsm_example_fsm::reset()
    {
        d_current_state = d_initial_state;
        d_input_symbol = "";
    }


    void 
    fsm_example_fsm::add_transition(std::string input_symbol, std::string state,
        fsm_example_fsm::type_action fn_action, std::string next_state, 
        type_condition fn_cond, std::string comment)
    {
      from_state frstt = make_tuple(input_symbol, state); 
      to_state tostt = make_tuple(fn_action, next_state, fn_cond,
        comment);
      d_state_transitions.insert(make_pair(frstt, tostt));
    }


    std::string 
    fsm_example_fsm::get_state()
    {
      std::string msg ="...fsm_example_fsm state: input symbol: " + d_input_symbol +
        //", initial state " + d_initial_state + 
        ",  current state " + d_current_state + "\n";
      return msg;
    }


    std::string
    fsm_example_fsm::get_transition(from_state frstt, to_state tostt)
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
    fsm_example_fsm::print_transitions()
    {
      std::cout << "...fsm_example_fsm transitions:" << std::endl;
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
    fsm_example_fsm::search_trans_print(from_state stt_search)
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


    /** Receives symbol, executes transition, moves machine.
     *
     * Receives a symbol, looks for a transition valid for the current state, verifies condition, executes action and returns result.
     * @param input_symbol the received symbol.
     * @param message a string message.
     * @param block a pointer to the main block to which the FSM is attached.
     * @return FSM transition action result.
     */
    std::string
    fsm_example_fsm::process(std::string input_symbol, std::string message,
      std::string block)
    {
      d_input_symbol = input_symbol;  // only for printing
      from_state stt_search;
      d_action_result = "";           // for fresh result

      // search for ordinary transitions
      stt_search = std::make_tuple(input_symbol, d_current_state);
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (symbol, state) transition: (" << 
            d_input_symbol << ", " << d_current_state << ")\n";
        }
      return d_action_result;
      }

      // search for transitions for any input symbol
      stt_search = std::make_tuple("", d_current_state);
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, state) transition" << 
            d_input_symbol << ", " << d_current_state << ")\n";
         }
        return d_action_result;
      }
      // search for default transition
      stt_search = std::make_tuple("", "");
      if ( exec_transition(stt_search) ) {
        if ( d_debug ) {
          std::cout << 
            "  Executed (any symbol, any state) transition" <<
            d_input_symbol << ", " << d_current_state << ")\n";
        return d_action_result;
        }
      }
      else
      {
        std::cout << "Default transition not found" << std::endl;
        throw ExceptionFSM("fsm_example_fsm EXCEPTION: no valid transitions!");
      }
    }


    bool
    fsm_example_fsm::exec_transition(from_state stt_search)
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
            fsm_example_fsm::type_action fn_action = std::get<0>(tostt);
            fn_action( *this);  // execute action

            d_current_state = std::get<1>(tostt); // set to next state
            if (d_debug) {
              std::cout << "  Executed transition: "; 
              std::cout << get_transition(frstt, tostt);
              std::cout << get_state();
            }
            return true;
          }  // end evaluate condition
        }  // end for
        return false;
      }
      return false;
    }


    void
    fsm_example_fsm::set_debug(bool dbg)
    {
      d_debug = dbg;
    }


  } /* namespace gwncppvgb */
} /* namespace gr */


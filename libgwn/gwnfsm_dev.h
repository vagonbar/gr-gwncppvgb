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


#ifndef INCLUDED_GWNCPPVGB_GWNFSM_DEV_H
#define INCLUDED_GWNCPPVGB_GWNFSM_DEV_H

#include <gwncppvgb/api.h>

/* GWN inclusions */
#include <iostream>     // for cin cout support
#include <string>       // for string concatenation
#include <sstream>      // for to_string
#include <pmt/pmt.h>    // for messages

#include <exception>
#include <tuple>
#include <map>
#include <stack>


namespace gr {
  namespace gwncppvgb {

    class gwnfsm_dev;  // FSM class name forward declaration, for typedef

    /*!
     * \brief A block implementing an FSM
     *
     */
    class GWNCPPVGB_API gwnfsm_dev
    {
    public:

      /** action function type definition. */
      typedef void (*type_action)(gwnfsm_dev &fsm);
      /** condition function type definition. */
      typedef bool (*type_condition)(gwnfsm_dev &fsm);
      /** from_state : (input symbol, current state). */
      typedef std::tuple<std::string, std::string> from_state;
      /** to_state   : (type_action, next state, condition, comment). */
      typedef std::tuple<type_action, std::string, type_condition, std::string> to_state;
      /** memory object type definition. */
      typedef pmt::pmt_t mem_type;


      ///////////////////////////////////
      // user defined variables and functions

      std::string where = "A";
      bool to_c = false;

      /** Usr defined function, sets user defined attribute.
       *
      * @param value: a string to set where value.
      */
      void set_where(std::string value);


      // end user defined public attributes and functions
      ///////////////////////////////////



      /**
       * \brief GWN FSM constructor.
       *
       * @param initial_state: the FSM initial state.
       */ 
       gwnfsm_dev(std::string initial_state);


      /**
       * \brief GWN FSM destructor.
       */  
        ~gwnfsm_dev();


        //std::string process(std::string msg);   // preliminary tests


      /** \brief Brings the machine back to its initial state.
       *
       * Sets the current state to the initial state and sets the input symbol to null. WARNING: memory is left untouched.
       */
      void reset();


      /** Adds a transition.

       This function adds a transition from the current state to another state. The transition is expressed through the association:
           (input_symbol, current_state) --> (action, next_state, condition)
       Several transitions may be found for a pair (input_symbol, current_state); the first of these transitions on which condition evaluates to true will be the one executed.
       @param input_symbol: the received symbol.
       @param state: the current state.
       @param action: a function to execute on transition. There must exist an action function, even if it does nothing.
       @param next_state: the state to which the machine will be moved and made the current state. If next_state is null, the current state will remain unchanged.
       @param condition: a function which returns true or false; if true, transition is performed, otherwise the transition is ignored, i.e. the FSM remains in its state and action is not executed. There must be always a condition, even if it just returns true.
      @param comment: an optiona comment to describe the transition and help interpret it when printed.
       */
      void add_transition(std::string input_symbol, std::string state,
        type_action fn_action, std::string next_state, 
        type_condition fn_condition, std::string comment = "");


      /** Shows an FSM transition.

      Prints a transition, an entry of the transitions table.
      @param frstt: key of multimap.
      @param tostt: value of multimap.
      @return a string describing a transition.
      */
      std::string get_transition(from_state frstt, to_state tostt);
      //std::string get_transition(from_state frstt, to_state tostt);


      /** Shows FSM current state.

      @return a string describing FSM state.
      */
      std::string get_state();


      /** Prints all the FSM transitions.
      */
      void print_transitions();


      /** Searches a transition from a (symbol, state) tuple.
      * 
      * @param stt_search: a (symbol, state) tuple to search transition with this from_state.
      */
      void search_trans_print(from_state stt_search);


      /** Searches and executes a transition.
      *
      * Searches a transition for (symbol, current_state) which condition evaluates to true, and if found executes action function and makes next state the current state (moves the machine). 
      @param input_symbol: the input symbol received.
      @param message: a string to pass on to the action function.
      @param block: a reference to the block to which the FSM is attached, to pass on to action functions. NOT IMPLEMENTED.
      @return the value returned by action, if any.
      */
      std::string process(std::string input_symbol, std::string message, std::string block);

      /** Executes a transition if condition is true.
       *
       * Returns true if condition was true and transitions was executed, false otherwise.
       */
      bool exec_transition(from_state stt_search); 


      /** Sets variable for debug messages */
      void set_debug(bool dbg);

      /** A null action function, when no action is required */
      void null_action(gwnfsm_dev fsm);

      /** A null condition function, when no action is required */
      bool null_condition(gwnfsm_dev fsm);


      /** Memory, push element into stack 
       *
       * @param obj  object to push into stack.
       */
      void mem_push(mem_type obj);
      /** Memory, removes element from stack */
      void mem_pop(); 
      /** Memory, test if stack is empgy */
      bool mem_empty();
      /** Memory, size of stack
       *
       * @return returns true if stack is empty.
       */
      int mem_size();
      /** Memory, get top element of stack, does not erase
       *
       * @return number of objects in stack.
       */
      mem_type mem_top();


      std::string d_action_result; /** action function result */
      void add_myfsm_transitions(); /** add user defined fSM transitions */

    private:

    /** FSM exception class declaration. */
    class ExceptionFSM
      {
        public:
          /** FSM exception constructor. */
          ExceptionFSM(std::string value);
          /** FSM exception destructor. */
          ~ExceptionFSM();
          /** Get exception message. */
          std::string get_value();
        private:
          std::string d_value;  /** Exception message. */
      };


      std::multimap<from_state, to_state> d_state_transitions; /** a multimap (from_state, to_state) */
      std::string d_default_transition; /** a transition for a invalid input, optional */
      std::string d_input_symbol; /** the received symbol */
      std::string d_initial_state; /** the initial state  */
      std::string d_current_state; /** the current state */
      std::string d_next_state; /** the state to go in a transition */
      type_action d_action; /** a function to execute on transition */
      std::stack<mem_type> d_memory; /** a LIFO stack */
      bool d_debug; /** If true prints debug messages */


    };

  } // namespace gwncppvgb
} // namespace gr

#endif /* INCLUDED_GWNCPPVGB_GWNFSM_DEV_H */


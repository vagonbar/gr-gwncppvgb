//* GWN Finite State Machine

// header guards
#ifndef FSM_H
#define FSM_H

// using standard exceptions
#include <iostream>
#include <exception>
#include <string>
#include <tuple>
#include <map>




/* FSM type declarations */
  //   from_state : (input symbol, current state) 
  //   to_state   : (condition, action, next state)
  typedef std::tuple<std::string, std::string> from_state;
  typedef std::tuple<std::string, std::string, std::string> to_state;


/* FSM exception declarations */

class ExceptionFSM
  {
    public:
      ExceptionFSM(std::string value);
      ~ExceptionFSM();
      std::string get_value();
    private:
    std::string d_value;
  };


class FSM
  {
  public:

  /** \brief A Finite State Machine (FSM) engine.

This module implements a Finite State Machine (FSM) engine. In addition to the usual states and transitions, the GWN FSM includes actions, memory, and conditions. 

An action is a user written function executed on a transition, before moving the machine to the next state.

TODO: Memory may be any object capable of recording and retrieving information, in whatever access mode the application may need (LIFO, FIFO, etc). The memory facility is not part of the FSM machine, but an independent object. Memory may be handled in the action functions.

A conditions is a user written function [or expression?] which returns true or false when executed or evaluated. The action function and the transition are only executed if the condition evaluates to true. If the condition on a transition evaluates to false, the transition is not performed, and its related action is not executed.

The FSM is defined through tables of transitions. In a current state, for a given input symbol, the process() method uses these tables to decide which action to call and which the next state will be, if and only if the condition evaluates to true; otherwise, nothing happens.

The table of transitions defines the following associations::

        (input_symbol, current_state) --> (action, next_state, condition)

where action is a function, symbols and states are strings, and condition is a function [or an expression] whith returns a boolean. This table is created through the FSM methods add_transition() [and add_transition_list() ].

A transition with the null string as an input_symbol ('') stands for a transition valid for any input symbol.

TODO, DEPRECATED: A second table of transitions defines another kind of association:
        (current_state) --> (action, next_state, condition)
This allows to add transitions valid for any input symbol. The table of any symbol transitions is maintained through the FSM method add_transition_any().

The FSM accepts one default transition not associated with any specific input_symbol or state. The default transition matches any symbol on any state, and may be used as a catch-all transition. The default transition is set through the set_default_transition() method. There can be only one default transition.

On receiving a symbol, 
  1. The FSM searches the transition table for a key (input_symbol, current_state). If one or more transitions are found, the condition is evaluated; if the condition is true, the transition is executed, otherwise, the remaining transitions are tested for condition, executing the first that is true.
  2. If no conditions in any of the found transitions are true, or no transitions were found, the FSM searches the transition table for a key ('', current_state), which stands for a transition valid for any input symbol. If one or more are found, their conditions are evaluated, and the first condition which returns true is executed.
  3. If no transitions with true conditions were found so far, the default transition is executed, if one exists.
  4. If no valid transition is found, the FSM will raise an exception.

Please note a search with the former criteria may produce several tuples (action, next_state, condition). The condition is evaluated for each tuple, and the first tuple on which the condition is found true is executed, which means the action function is called, and the next state is set as the current state.

If no transition is defined for an input symbol, the FSM will raise an exception. This can be prevented by defining a default transition. 

The action function receives a reference to the FSM as a parameter, hence the action function has access to all attributes in the FSM, such as current_state, input_symbol or memory.

The GWN Finite State Machine implementation is an extension of Noah Spurrier's FSM 20020822, C{http://www.noah.org/python/FSM/}.
  */
    FSM(std::string initial_state);

    ~FSM();


    void reset();

     /** Adds a transition.

        This function adds a transition from the current state to another state. The transition is expressed through the association::

           (input_symbol, current_state) --> [ (action, next_state, condition) ]

        On the destination list, the first transition where condition returns True will be the one executed.

        Transitions for a list of symbols may be added with the function add_transition_list().
        @param input_symbol: the received symbol.
        @param state: the current state.
        @param action: a function to execute on transition. This action may be set to None in which case the process() method will ignore the action and only set the next_state. This parameter may be given as a list of functions.
        @param next_state: the state to which the machine will be moved and made the current state. If next_state is None, the current state will remain unchanged.
        @param condition: a function or expression which returns True or False; if True, transition is performed, otherwise the transition is ignored, i.e. the FSM remains in its state and action is not executed. If this parameter is None, no conditions are checked, and transition is performed. This parameter may be given as a list of expressions or functions.
      */
    void add_transition(std::string input_symbol, std::string state,
      std::string condition, std::string action, std::string next_state);

    void print_transition(from_state, to_state);
    void print_state();
    void print_transitions();
    void search_trans_print(from_state stt_search);

    // a map { (input symbol, current state): 
    //   (condition, action, next state) }
    std::multimap<from_state, to_state> d_state_transitions;
  private:
    std::string d_default_transition;
    std::string d_input_symbol;
    std::string d_initial_state;
    std::string d_current_state;
    std::string d_next_state;
    // action = None
    // memory = memory
    bool d_debug;


  };


#endif


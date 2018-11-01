// fsm.h : GWN Finite State Machine

/**
 * @file fsm.h
 * @brief A finite state machine (FSM). 
 * @author Victor Gonzalez-Barbone
 * @date 9 Nov 2018. 
 *
 * This module implements a Finite State Machine (FSM) engine. In addition to the usual states and transitions, the GWN FSM includes actions, memory, and conditions. 

An action is a user written function executed on a transition, before moving the machine to the next state.

TODO: Memory may be any object capable of recording and retrieving information, in whatever access mode the application may need (LIFO, FIFO, etc). The memory facility is not part of the FSM machine, but an independent object. Memory may be handled in the action functions.

A conditions is a user written function [or expression?] which returns true or false when executed or evaluated. The action function and the transition are only executed if the condition evaluates to true. If the condition on a transition evaluates to false, the transition is not performed, and its related action is not executed.

The FSM is defined through tables of transitions. In a current state, for a given input symbol, the process() method uses these tables to decide which action to call and which the next state will be, if and only if the condition evaluates to true; otherwise, nothing happens.

The table of transitions defines the following associations:

        (input_symbol, current_state) --> (action, next_state, condition)

where action is a function, symbols and states are strings, and condition is a function [or an expression] whith returns a boolean. This table is created through the FSM methods add_transition() [and add_transition_list() ].

A transition with the null string as an input_symbol ('') stands for a transition valid for any input symbol.

TODO, DEPRECATED: A second table of transitions defines another kind of association:
        (current_state) --> (action, next_state, condition)
This allows to add transitions valid for any input symbol. The table of any symbol transitions is maintained through the FSM method add_transition_any().

The FSM accepts one default transition not associated with any specific input_symbol or state. The default transition matches any symbol on any state, and may be used as a catch-all transition.The default transition is set through the set_default_transition() method, which inserts a transition with from_state = ("", "") . There can be only one default transition.

On receiving a symbol, 
  1. The FSM searches the transition table for a key (input_symbol, current_state). If one or more transitions are found, the condition is evaluated for each transition in order. The first transition on which condition is true is executed.
  2. If no conditions in any of the found transitions are true, or no transitions were found, the FSM searches the transition table for a key ("", current_state), which stands for a transition valid for any input symbol. If one or more are found, their conditions are evaluated, and the first condition which returns true is executed.
  3. If no transitions with true conditions were found so far, the FSM searches for a default transition and executes if one is found it is executed.
  4. If no valid transition was found, the FSM will raise an exception.

Please note a search with the former criteria may produce several tuples (action, next_state, condition). The condition is evaluated for each tuple, and the first tuple on which the condition is found true is executed, which means the action function is called, and the next state is set as the current state.

If no transition is defined for an input symbol, the FSM will raise an exception. This can be prevented by defining a default transition. 

The action function receives a reference to the FSM as a parameter, hence the action function has access to all attributes in the FSM, such as current_state, input_symbol or memory.

The GWN Finite State Machine implementation is an extension of Noah Spurrier's FSM 20020822, C{http://www.noah.org/python/FSM/}.

*/


#ifndef FSM_H
#define FSM_H

#include <iostream>
#include <exception>
#include <string>
#include <tuple>
#include <map>



  /**   from_state : (input symbol, current state) */
  typedef std::tuple<std::string, std::string> from_state;
  /**   to_state   : (condition, action, next state) */
  typedef std::tuple<std::string, std::string, std::string> to_state;


/** FSM exception class declaration */
class ExceptionFSM
  {
    public:
      /** FSM exception constructor */
      ExceptionFSM(std::string value);
      /** FSM exception destructor */
      ~ExceptionFSM();
      /** Get exception message */
      std::string get_value();
    private:
      std::string d_value;  /** Exception message  */
  };


/** \brief GWN Finite State Machine (GWN-FSM) with string as symbols.

  The GWN FSM uses strings as input symbols. It uses a multimap

      { (input symbol, current state): [ (action, next state, condition) ] }

  which defines a transition from the current state when a certain symbol is received.
*/
class FSM
  {
  public:

    /**
     * \brief GWN FSM constructor.
     *
     * @param initial_state: the FSM initial state.
     * @param memory:  an object intended to pass along to the action functions. A usual option is a list used as a stack.
     */ 
    FSM(std::string initial_state);


    /**
     * \brief GWN FSM destructor.
     */  
    ~FSM();


    /** \brief Brings the machine back to its initial state.
     *
     * Sets the current state to the initial state and sets input_symbol to null. WARNING: memory is left untouched.
     */
    void reset();


    /** Adds a transition.

     This function adds a transition from the current state to another state. The transition is expressed through the association:

         (input_symbol, current_state) --> (action, next_state, condition)

     Several transitions may be found for a pair (input_symbol, current_state); the first of these transitions on which condition evaluates to true will be the one executed.
     @param input_symbol: the received symbol.
     @param state: the current state.
     @param condition: a function which returns true or false; if true, transition is performed, otherwise the transition is ignored, i.e. the FSM remains in its state and action is not executed. If this parameter is null no conditions are checked and the transition is performed.
     @param action: a function to execute on transition. This action may be set to null in which case the process() method will ignore the action and only set the next_state.
     @param next_state: the state to which the machine will be moved and made the current state. If next_state is null, the current state will remain unchanged.
     */
    void add_transition(std::string input_symbol, std::string state,
      std::string condition, std::string action, std::string next_state);


    /** Sets a default transition.

    The default transition can be removed by setting the attribute default_transition to null.
    @param action: a function to execute on transition. This action may be set to null, in which case the process() method will ignore the action and only set the next_state.
    @param next_state: the state to which the machine will be moved and made the current state. If next_state is null, the current state will remain unchanged.
    */
    //    self.default_transition = [ (action, next_state, None) ]
    void set_default_transition (std::string action, 
      std::string next_state);


    /** Prints an FSM transition.

    Prints a transition, an entry of the transitions table.
    @param frstt: key of multimap.
    @param tostt: value of multimap.
    */
    void print_transition(from_state frstt, to_state tostt);


    /** Prints FSM current state.
    */
    void print_state();


    /** Prints all the FSM transitions.
    */
    void print_transitions();


    /** Searches a transition from a symbol, state.
    * 
    * @param stt_search: a (symbol, state) to search transition with this from_state.
    */
    void search_trans_print(from_state stt_search);


    /** Searches and executes a transition.
    *
    * Searches a transition for (symbol, current_state) which condition evaluates to true, and if found executes action function and makes next state the current state (moves the machine). 
    @param input_symbol: the input symbol received.
    @param message: a string to pass on to the action function.
    @param block: a reference to the block to which the FSM is attached, to pass on to action functions.
    @return the value returned by action, if any.
    */
    void process(std::string input_symbol, std::string message,
      std::string block);

  private:

    std::multimap<from_state, to_state> d_state_transitions; /** a multimap (from_state, to_state) */
    std::string d_default_transition; /** a transition for a invalid input, optional */
    std::string d_input_symbol; /** the received symbol */
    std::string d_initial_state; /** the initial state  */
    std::string d_current_state; /** the current state */
    std::string d_next_state; /** the state to go in a transition */
    // d_action = None /** a function to execute on transition */
    // d_memory = memory /** a LIFO list */
    bool d_debug; /** If true prints debug messages */


  };


#endif


// fsm.h : GWN Finite State Machine

/**
 * @file fsm.h
 * @brief A finite state machine (FSM). 
 * @author Victor Gonzalez-Barbone
 * @date 9 Nov 2018. 
 *
 * This module implements a Finite State Machine (FSM) engine. In addition to the usual states and transitions, the GWN FSM includes actions, conditions, and memory. 

The FSM is defined by transitions. For an input symbol, according to the current state in which the machine is, if there is a transition for the pair (symbol, current state), the condition is evaluated, and if found true, the action is executed and the next state is made the current state (the machine is moved to the next state).

An action is a user written function executed on a transition, before moving the machine to the next state. The action function, as well as the transition, are only executed if the condition function for the transition is true.

A condition is a user written function which returns true or false when executed or evaluated. The action function and the transition are only executed if the condition evaluates to true. If the condition evaluates to false, the transition is not performed, and its related action is not executed. The use of action functions is optional, since actions can be coded in condition functions. In this case, care must be taken in returning false before the actions are executed, to ensure the actions will only be executed if the condition evaluates to true.

Memory is implemented as a stack and is accessed as a LIFO (Last In First Out) container. A type definition describes the type of data stored in the FSM memory. By changing the data type definition, different types of data may be stored. Memory functions in the FSM machine allow to push, pop, determine size of the stack, and if it is empty.

The FSM is implemented as a table of transitions. For a given input symbol and according to the current state the machine is in, the process() method searches the table for a transition corresponding to the pair (input symbol, current state). If one or more transitions are found, the condition functions are evaluated. The first transition where the condition is true is executed, which means the action is executed and the next state is made the current state (the machine is moved to the next state). If no transitions are found or no conditions are true on the found transitions, nothing happens; the action is not excecuted, and the current state is not changed.

The table of transitions defines the following associations:

        (input_symbol, current_state) --> (action, next_state, condition)

where action is a function, symbols and states are strings, and condition is a function which returns a boolean. This table is created through the FSM methods add_transition().

A transition with the null string as an input_symbol ('') stands for a transition valid for any input symbol. For the current state, if the condition is true, the transition will be executed on receiving any symbol.

The FSM accepts one default transition not associated with any specific input symbol or state, i.e. valid for any input symbol and any current state. The default transition may be used as a catch-all transition. The default transition is set by insertin a transition with null strings as input symbol and current state, i.e. a pair ("", ""). There should be only one default transition; if there are several, the first one found will be executed.

The FSM machine operates in the following way. On receiving a symbol, 
  1. The FSM searches the transition table for a key (input symbol, current state). If one or more transitions are found, the condition is evaluated for each transition in order. The first transition on which condition is true is executed.
  2. If no conditions in any of the found transitions are true, or no transitions were found, the FSM searches the transition table for a key ("", current state), which stands for a transition valid for any input symbol. If one or more are found, their conditions are evaluated, and the first condition which returns true is executed.
  3. If no transitions with true conditions were found so far, the FSM searches for a default transition and executes it if one is found.
  4. If no valid transition was found, i.e. no default transition exists, the FSM will raise an exception.

The action functions and the condition functions receive a reference to the FSM object as a parameter, hence the action function has access to all attributes in the FSM, such as current state, input symbol or memory.

The GWN Finite State Machine implementation is an extension of Noah Spurrier's FSM 20020822, C{http://www.noah.org/python/FSM/}.

*/


#ifndef FSM_H
#define FSM_H

#include <iostream>
#include <exception>
#include <string>
#include <tuple>
#include <map>
#include <stack>


class FSM;  // forward declaration, for typedef


/** action function type definition */
typedef void (*type_action)(FSM &fsm);
/** condition function type definition */
typedef bool (*type_condition)(FSM &fsm);
/** from_state : (input symbol, current state) */
typedef std::tuple<std::string, std::string> from_state;
/** to_state   : (type_action, next state, condition, comment) */
typedef std::tuple<type_action, std::string, type_condition, std::string> to_state;
/** memory object type definition */
typedef std::string mem_type;


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


/** \brief GWN Finite State Machine (GWN FSM) with string as symbols.

  The GWN FSM uses strings as input symbols. It uses a multimap
      { (input symbol, current state): (action, next state, condition) }
  which defines a transition from the current state when a certain symbol is received.
*/
class FSM
  {
  public:

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
    FSM(std::string initial_state);


    /**
     * \brief GWN FSM destructor.
     */  
    ~FSM();


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
    void process(std::string input_symbol, std::string message,
      std::string block);

    /** Executes a transition if condition is true.
     *
     * Returns true if condition was true and transitions was executed, false otherwise.
     */
    bool exec_transition(from_state stt_search); 


    /** Sets variable for debug messages */
    void set_debug(bool dbg);

    /** A null action function, when no action is required */
    void null_action(FSM fsm);

    /** A null condition function, when no action is required */
    bool null_condition(FSM fsm);


    /** Memory, push element into stack 
     *
     * @param obj  object to push into stack.
     */
    void mem_push(mem_type obj);
    /** Memory, removes element from stack */
    void mem_pop(); 
    /** Memory, stack is empgy */
    bool mem_empty();
    /** Memory, size of stack */
    int mem_size();
    /** Memory, get top element of stack, does not erase
     *
     * @return number of objects in stack.
     */
    mem_type mem_top();


  private:

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


#endif


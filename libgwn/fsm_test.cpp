/** GWN FSM test
 *
 * GWN Finite State Machine test.
 */

#include "fsm.h"


/*  Actions */


/* Condition functions */




int main () {


  /* check exceptions */
  std::cout << "i\n=== check exceptions" << std::endl;
  try
  {
    throw ExceptionFSM("ExceptionFSM happened!");
    //ExceptionFSM myex2(3);
    //throw myex2;
  }
  //catch (std::exception& e)
  catch (ExceptionFSM e)
  {
    std::cout << "  e.get_value(): " << e.get_value() << "\n";
  }


  FSM myfsm = FSM("INIT");


  /* States
   *  load FSM transitions table, show transitions
   */

  // initial conditions
    std::string where = "A"; 
    bool to_c = false;
    //myfsm.debug = true;

  // add_transition parameters:
  //   input symbol, current state, action, next state, condition
  //   no condition ~ true

  // default transition
    myfsm.add_transition ("", "", "fn_error", "INIT", "true");

  // transitions for any input symbol
    myfsm.add_transition ("", "INIT", "", "STATE A", "true");
    myfsm.add_transition ("", "STATE A", "fn_none", "STATE A", "true");

  // add transitions
    myfsm.add_transition ("s", "INIT", "show", "INIT", "true");

  //myfsm.add_transition("c", "INIT", "show", "INIT", "false");
  myfsm.add_transition ("g", "INIT", "fn_goA", "STATE A", "where==A");
  myfsm.add_transition ("g", "INIT", "fn_goB", "STATE B", "where==B");
  myfsm.add_transition ("g", "INIT", "[fn_goA, fn_goB]", "STATE C", "where==C && cn_toc()");
  myfsm.add_transition ("r", "STATE A", "fn_init", "INIT", "true");
  myfsm.add_transition ("r", "STATE B", "fn_init", "INIT", "true");
  myfsm.add_transition ("r", "STATE C", "fn_init", "INIT", "true");

  myfsm.add_transition ("w", "INIT", "fn_chgwhr", "CHG WHERE", "false");
  myfsm.add_transition ("c", "INIT", "fn_chgtoC", "CHG TOC", "false");
  myfsm.add_transition ("r", "CHG WHERE", "fn_init", "INIT", "false");
  myfsm.add_transition ("r", "CHG TOC", "fn_init", "INIT", "false");

  std::cout << "\n=== FSM state and transitions" << std::endl; 
  myfsm.print_state();
  myfsm.print_transitions();



  /*  test search transitiosn by symbol, state, condition */

  std::cout << "\n--- search transition (r, STATE C):" << std::endl; 
  //from_state state0 = std::make_tuple("w", "INIT", false);
  from_state state0 = std::make_tuple("r", "STATE C");
  myfsm.search_trans_print(state0);

  std::cout << "\n--- search transition (x, xxxx):" << std::endl; 
  from_state state1 = std::make_tuple("x", "xxxx");
  myfsm.search_trans_print(state1);


  myfsm.add_transition ("r", "STATE C", "fn_init", "INIT", "false");
  std::cout << "\n--- search transition (r, STATE C):" << std::endl; 
  from_state state2 = std::make_tuple("r", "STATE C");
  myfsm.search_trans_print(state2);


  return 0;
}


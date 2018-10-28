/*
 * fsm_test.cpp
 * GWN Finite State Machine test
 */

#include "fsm.h"

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


  /* load FSM transitions table, show transitions */

  FSM myfsm = FSM("INIT");

  // add transitions
  // input symbol, current state, condition, action, next state
  myfsm.add_transition("c", "INIT", "show", "INIT", "false");
  myfsm.add_transition("g", "INIT", "fn_goA", "State A", "false"); //"where=="A"");
  myfsm.add_transition("g", "INIT", "fn_goB", "State B", "false"); //"where=="B"");
  myfsm.add_transition ("g", "INIT", "[fn_goA, fn_goB]", "State C", "false"); //  ["self.where=="C"", cn_toc])
  myfsm.add_transition ("r", "State A", "fn_init", "INIT", "false");
  myfsm.add_transition ("r", "State B", "fn_init", "INIT", "false");
  myfsm.add_transition ("r", "State C", "fn_init", "INIT", "false");

  myfsm.add_transition ("w", "INIT", "fn_chgwhr", "Chg Where", "false");
  myfsm.add_transition ("c", "INIT", "fn_chgtoC", "Chg ToC", "false");
  myfsm.add_transition ("r", "Chg Where", "fn_init", "INIT", "false");
  myfsm.add_transition ("r", "Chg ToC", "fn_init", "INIT", "false");

  std::cout << "\n=== FSM state and transitions" << std::endl; 
  myfsm.print_state();
  myfsm.print_transitions();


  /*  search transitiosn by symbol, state, condition */
  std::cout << "\n--- search transition" << std::endl; 
  //from_state state0 = std::make_tuple("w", "INIT", false);
  from_state state0 = std::make_tuple("r", "State C");
  myfsm.search_trans_print(state0);

  from_state state1 = std::make_tuple("x", "xxxx");
  myfsm.search_trans_print(state1);


  myfsm.add_transition ("r", "State C", "fn_init", "INIT", "false");
  from_state state2 = std::make_tuple("r", "State C");
  myfsm.search_trans_print(state2);


  return 0;
}


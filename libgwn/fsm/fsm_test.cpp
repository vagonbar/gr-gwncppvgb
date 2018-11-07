/** GWN FSM test
 *
 * GWN Finite State Machine test.
 */

#include "fsm.h"


/*  initial conditions as global variables */
//std::string where = "A"; 
//bool to_c = false;


/* Utility functiones */

void show_add_attrs(FSM &fsm) {  // show additional atributes
    std::cout << "  --- FSM show_add_attrs; fsm.where=" << 
      fsm.where << ", fsm.to_c=" << fsm.to_c << std::endl;
}



/*  Action functions */


void fn_none(FSM &fsm) {
    std::cout << "  --- FSM none, nothing done" << std::endl;
    return;
}


void fn_show(FSM &fsm) {
    std::cout << "  --- FSM memory show and erase, INIT state" << std::endl;
    std::cout << "      Memory size: " << fsm.mem_size() <<
      ", contents:" << std::endl;
  while (!fsm.mem_empty())
  {
     std::cout << " : " << fsm.mem_top();
     fsm.mem_pop();
  }
  std::cout << std::endl;
    return;
}

void fn_goA(FSM &fsm) {
    //fsm.where = "B";
    //fsm.set_where("B");
    fsm.mem_push("A visited!");
    std::cout << "  --- FSM fn_goA" << fsm.where << std::endl;
    return;
}

void fn_goB(FSM &fsm) {
    //fsm.where = "C";
    fsm.mem_push("B visited!");
    std::cout << "  --- FSM fn_goB" << std::endl;
    return;
}

void fn_goC(FSM &fsm) {
    //fsm.where = "A";
    fsm.mem_push("C visited!");
    std::cout << "  --- FSM fn_goC" << fsm.where << std::endl;
    return;
}

void fn_goAB(FSM &fsm) {
  fn_goA(fsm);
  fn_goB(fsm);
  fsm.mem_push("C visited!");
}


void fn_init(FSM &fsm) {
    std::cout << "  --- FSM fn_init" << std::endl;
    return;
}

void fn_chgtoC(FSM &fsm) {
    if (fsm.to_c == true )
      fsm.to_c = false; 
    else
      fsm.to_c = true;
    std::cout << "  --- FSM fn_toC" << ", to_c set to " << fsm.to_c << std::endl;
    return;
}

void fn_chgwhr(FSM &fsm) {
    if (fsm.where == "A")
        fsm.where = "B";
    else if (fsm.where == "B")
        fsm.where = "C";
    else
        fsm.where = "A";
    std::cout << "  --- FSM fn_init, fsm.where set to " << fsm.where << std::endl;
    return;
}

void fn_error(FSM &fsm) {
    std::cout << "  --- FSM error " << std::endl;
}



/* Condition functions */

bool cnd_true(FSM &fsm) {
  return true;
}

bool cnd_false(FSM &fsm) {
  return false;
}

bool cnd_A(FSM &fsm) {
  return fsm.where == "A";
}

bool cnd_B(FSM &fsm) {
  return fsm.where == "B";
}

bool cnd_C(FSM &fsm) {
  return fsm.where == "C";
}

bool cnd_C_to_c(FSM &fsm) {
  return fsm.where == "C" && fsm.to_c == true;
}



int main () {


  FSM myfsm = FSM("INIT");     // FSM machine, construction
  myfsm.set_debug(true);
  myfsm.where = "A";
  myfsm.to_c = false;

  /** States: load FSM transitions table, show transitions
   */

  // add_transition parameters:
  //   input symbol, current state, action, next state, condition
  //   no condition ~ true

  // default transition
  myfsm.add_transition ("", "", fn_error, "INIT", cnd_true, "default transition");

  // transitions for any input symbol
  myfsm.add_transition ("", "INIT", fn_none, "INIT", cnd_true,
      "any symbol");
  myfsm.add_transition ("", "STATE_A", fn_none, "STATE_A", cnd_true,
      "any symbol");

  // add ordinary transitions
  myfsm.add_transition ("s", "INIT", fn_show, "INIT", cnd_true);

  myfsm.add_transition ("g", "INIT", fn_goA, "STATE_A", cnd_A, "where==A");
  myfsm.add_transition ("g", "INIT", fn_goB, "STATE_B", cnd_B, "where==B");
  myfsm.add_transition ("g", "INIT", fn_goAB, "STATE_C", cnd_C_to_c, 
    "where==C && to_c" ); 

  myfsm.add_transition ("r", "STATE_A", fn_init, "INIT", cnd_true);
  myfsm.add_transition ("r", "STATE_B", fn_init, "INIT", cnd_true);
  myfsm.add_transition ("r", "STATE_C", fn_init, "INIT", cnd_true);

  myfsm.add_transition ("w", "INIT", fn_chgwhr, "CHG_WHERE", cnd_true);
  myfsm.add_transition ("c", "INIT", fn_chgtoC, "CHG_TOC", cnd_true);
  myfsm.add_transition ("r", "CHG_WHERE", fn_init, "INIT", cnd_true);
  myfsm.add_transition ("r", "CHG_TOC", fn_init, "INIT", cnd_true);

  std::cout << "\n=== FSM state and show transitions" << std::endl; 
  myfsm.print_state();
  show_add_attrs(myfsm);
  myfsm.print_transitions();


  std::cout << "\n=== FSM interactive process transitions" << std::endl; 
  //myfsm.where = "A";
  myfsm.reset();
  myfsm.print_state();


  //std::string smb = "q";  // does not run interactive process
  std::string smb = "";
  try
  {
    while ( smb != "q" )
    {
      std::cout << "\nType symbol, q to finish: ";
      std::cin >> smb;
      //std::cout << "  processing " << smb << ", myfsm.where=" << myfsm.where << std::endl;
      myfsm.process(smb, "", "");
      //std::cout << "  processed " << smb << ", myfsm.where=" << myfsm.where << std::endl;
      show_add_attrs(myfsm);
    }
  }
  catch (ExceptionFSM e)
  {
    std::cout << "Exception catched: " << e.get_value() << std::endl;
  }

  return 0;
}


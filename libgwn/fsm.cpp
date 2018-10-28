/*
 * fsm.cpp
 *  GWN Finite State Machine 
 */

#include "fsm.h"


/* FSM exception functions */

ExceptionFSM::ExceptionFSM(std::string value) 
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



/*  FSM class */

FSM::FSM(std::string initial_state)
  {
    d_default_transition = "";
    d_input_symbol = "";
    d_initial_state = initial_state;
    d_current_state = initial_state;
    d_next_state = "";
    // action = None
    // memory = memory
    d_debug = false;

  }
FSM::~FSM() {}

void
FSM::reset()
  {
    d_current_state = d_initial_state;
    d_input_symbol = "";
  }


void 
FSM::add_transition(std::string input_symbol, std::string state,
    std::string condition, std::string action, std::string next_state)
  {
  
    from_state frstt = make_tuple(input_symbol, state); 
    to_state tostt = make_tuple(condition, action, next_state);
    d_state_transitions.insert(make_pair(frstt, tostt));
    //d_state_transitions[frstt] = tostt;
    //d_state_transitions.insert(make_tuple(frstt, tostt));
  }

void FSM::print_transition(from_state frstt, to_state tostt)
  {
    std::cout << "  (" <<
      std::get<0>(frstt) << ", " << std::get<1>(frstt) << ") = (" <<
      std::get<0>(tostt) << ", " << std::get<1>(tostt) <<
      ", " << std::get<2>(tostt) << ")" << std::endl;
  }


void
FSM::print_state()
  {
    std::cout << "FSM state:" << std::endl;
    std::cout << "  initial state: " << d_initial_state << std::endl;
    std::cout << "  current state: " << d_current_state << std::endl;
    std::cout << "  input symbol : " << d_input_symbol << std::endl;
    std::cout << "  next state   : " << d_next_state << std::endl;
  }

void
FSM::print_transitions()
  {
    std::cout << "FSM transitions:" << std::endl;
    std::map<from_state, to_state>::const_iterator it; 
    it = d_state_transitions.begin();
    while (it != d_state_transitions.end()) 
    {
      // print transition
      from_state frstt = it->first;
      to_state tostt = it->second;
      print_transition(frstt, tostt);

      ++it;
    } 
    //d_state_transitions
    std::cout << std::endl;
  } 

void
FSM::search_trans_print(from_state stt_search)
  {
    auto range = d_state_transitions.equal_range(stt_search);
    auto ptr = range.first;
    auto range_end = d_state_transitions.end();
    //auto ptr_end = range_end.first;
    if ( ptr == range_end ) 
      {
        std::cout << "transitions not found.\n";
        return;
      }
    else
      std::cout << "transitions found:\n";

    for (auto i = range.first; i != range.second; ++i)
      {
        from_state frstt = i->first;
        to_state tostt = i->second;
        print_transition(frstt, tostt);
      }
  }



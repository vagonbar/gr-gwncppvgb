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
    //d_action = ???;
    // d_memory = memory
    d_debug = false;

  }
FSM::~FSM() {}


void
FSM::null_action(FSM fsm) {}

bool
FSM::null_condition(FSM fsm) {return true;}


void
FSM::reset()
  {
    d_current_state = d_initial_state;
    d_input_symbol = "";
  }


void 
FSM::add_transition(std::string input_symbol, std::string state,
    type_action fn_action, std::string next_state, 
    type_condition fn_cond, std::string comment)
  {
    from_state frstt = make_tuple(input_symbol, state); 
    to_state tostt = make_tuple(fn_action, next_state, fn_cond,
      comment);
    d_state_transitions.insert(make_pair(frstt, tostt));
  }


void
FSM::print_state()
  {
    std::cout << "  FSM state: input symbol: " << d_input_symbol <<
      ", initial state " << d_initial_state << 
      ",  current state " << d_current_state << std::endl;
  }


void FSM::print_transition(from_state frstt, to_state tostt)
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
FSM::print_transitions()
  {
    std::cout << "FSM transitions:" << std::endl;
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
FSM::search_trans_print(from_state stt_search)
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


void
FSM::process(std::string input_symbol, std::string message,
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
      return;
    }

    // search for transitions for any input symbol
    stt_search = std::make_tuple("", d_current_state);
    if ( exec_transition(stt_search) ) {
      if ( d_debug ) {
        std::cout << 
          "  Executed (any symbol, state) transition" << std::endl;
      }
      return;
    }
 
    // search for default transition
    stt_search = std::make_tuple("", "");
    if ( exec_transition(stt_search) ) {
      if ( d_debug ) {
        std::cout << 
          "  Executed (any symbol, any state) transition" << std::endl;
      return;
      }
    }
    else
    {
      std::cout << "Default transition not found" << std::endl;
      throw ExceptionFSM("FSM EXCEPTION: no valid transitions!");
    }
}


bool
FSM::exec_transition(from_state stt_search)
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
        if ( fn_cond( *this ) )               // eval condition
        {
          //std::cout << "exec debug, where=" << where <<
          //  " this->where=" << this->where << std::endl;
          type_action fn_action = std::get<0>(tostt);
          fn_action( *this);                    // execute action

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
FSM::set_debug(bool dbg)
{
    d_debug = dbg;
}



/* User defined functions */

void
FSM::set_where(std::string value)
{
  where = value;
}



/* FSM memory */

void
FSM::mem_push(mem_type obj) {
  d_memory.push(obj); 
}

void
FSM::mem_pop() {
  d_memory.pop(); 
} 

bool
FSM::mem_empty() {
  return d_memory.empty();
}

int
FSM::mem_size() {
  return d_memory.size();
}

mem_type
FSM::mem_top() {
  return d_memory.top();
}


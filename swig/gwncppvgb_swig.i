/* -*- c++ -*- */

#define GWNCPPVGB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gwncppvgb_swig_doc.i"

%{
#include "gwncppvgb/message_timer_example.h"
#include "gwncppvgb/symbol_strobe.h"
#include "gwncppvgb/fsm_example.h"
#include "gwncppvgb/fsm_example_fsm.h"
%}


%include "gwncppvgb/message_timer_example.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, message_timer_example);
%include "gwncppvgb/symbol_strobe.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, symbol_strobe);

%include "gwncppvgb/fsm_example.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, fsm_example);
%include "gwncppvgb/fsm_example_fsm.h"

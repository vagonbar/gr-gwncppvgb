/* -*- c++ -*- */

#define GWNCPPVGB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gwncppvgb_swig_doc.i"

%{
#include "gwncppvgb/symbol_strobe.h"
#include "gwncppvgb/message_timer_example.h"
#include "gwncppvgb/fsm_example.h"
#include "gwncppvgb/fsm_example_fsm.h"
#include "gwncppvgb/data_source.h"
#include "gwncppvgb/stop_wait_ack.h"
#include "gwncppvgb/virtual_channel.h"
%}

%include "gwncppvgb/symbol_strobe.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, symbol_strobe);
%include "gwncppvgb/message_timer_example.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, message_timer_example);
%include "gwncppvgb/fsm_example.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, fsm_example);
%include "gwncppvgb/fsm_example_fsm.h"

%include "gwncppvgb/data_source.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, data_source);
%include "gwncppvgb/stop_wait_ack.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, stop_wait_ack);
%include "gwncppvgb/virtual_channel.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, virtual_channel);


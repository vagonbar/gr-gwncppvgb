/* -*- c++ -*- */

#define GWNCPPVGB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gwncppvgb_swig_doc.i"

%{
#include "gwncppvgb/message_passer.h"
#include "gwncppvgb/message_passer_pdata.h"
#include "gwncppvgb/gwnfsm.h"
#include "gwncppvgb/gwntimer_strobe.h"
#include "gwncppvgb/message_repeat.h"
#include "gwncppvgb/message_repeat_pdata.h"
%}
%include "gwncppvgb/message_passer.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, message_passer);
%include "gwncppvgb/message_passer_pdata.h"
%include "gwncppvgb/gwnfsm.h"
%include "gwncppvgb/gwntimer_strobe.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, gwntimer_strobe);

%include "gwncppvgb/message_repeat.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, message_repeat);
%include "gwncppvgb/message_repeat_pdata.h"

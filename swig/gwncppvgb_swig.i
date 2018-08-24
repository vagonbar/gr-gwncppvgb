/* -*- c++ -*- */

#define GWNCPPVGB_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "gwncppvgb_swig_doc.i"

%{
#include "gwncppvgb/gwnblock.h"
#include "gwncppvgb/gwnblock_pdata.h"
#include "gwncppvgb/message_passer.h"
#include "gwncppvgb/message_passer_pdata.h"
%}

%include "gwncppvgb/gwnblock.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, gwnblock);

%include "gwncppvgb/gwnblock_pdata.h"
%include "gwncppvgb/message_passer.h"
GR_SWIG_BLOCK_MAGIC2(gwncppvgb, message_passer);
%include "gwncppvgb/message_passer_pdata.h"

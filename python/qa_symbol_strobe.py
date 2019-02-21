#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2018 <+YOU OR YOUR COMPANY+>.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this software; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks

# corrected, gr_modtool creation said gwncppvgb_swig, not found!
import gwncppvgb.gwncppvgb_swig as gwncppvgb

import time
import pmt

class qa_symbol_strobe (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None


    def test_ports_timer (self):
        print "\n===\n=== TEST symbol_strobe output \n===\n"
        pss = gwncppvgb.symbol_strobe( "abcdefgh", 10, 1000.0)
        dbg = blocks.message_debug() 
        self.tb.msg_connect( (pss, "out_port_0"), (dbg, "print") )
  
        self.tb.start ()
        time.sleep(10)
        self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_symbol_strobe, "qa_symbol_strobe.xml")

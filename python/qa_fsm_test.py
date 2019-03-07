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

class qa_fsm_test (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    #def test_constructor (self):
        #print "\n===\n=== TEST fsm_test constructor \n===\n"
        #gwncppvgb.set_debug(True)   # does now work as expected
        #myblock = gwncppvgb.fsm_test("GWN test message 1", 10)
        #print "myblock.__str__()", myblock.__str__()  # not as expected


        # set up fg
        #self.tb.run ()
        # check data
        #pass

    def test_fsm_debug (self):
        print "\n===\n=== TEST 1 FSM \n===\n"
        # full example FSM graph testing "gxyrwrgrwrcrgrxys"
        src = gwncppvgb.symbol_strobe("sgxyrwrgrwrcrgrxys", 19, 1000)
        pss = gwncppvgb.fsm_test(False)  # change to True for debug
        dbg = blocks.message_debug() 
        self.tb.msg_connect( (src, "out_port_0"), (pss, "in_port_0") )
        self.tb.msg_connect( (pss, "out_port_0"), (dbg, "print") )

        self.tb.start ()
        time.sleep(20)
        self.tb.stop()
        return

if __name__ == '__main__':
    gr_unittest.run(qa_fsm_test, "qa_fsm_test.xml")

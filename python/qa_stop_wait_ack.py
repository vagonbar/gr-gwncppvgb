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

class qa_stop_wait_ack (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_constructor (self):
        #print "\n===\n=== TEST stop_wait_ack constructor \n===\n"
        #myblock = gwncppvgb.stop_wait_ack("GWN test message 1", 10)
        # set up fg
        #self.tb.run ()
        # check data
        pass

    def test_ports_timer (self):
        print "\n===\n=== TEST stop_wait_ack input and output ports \n===\n"
        tst_msg = "--- A message from message strobe"
        src = gwncppvgb.data_source("Data", "Data", "Data payload", 500.0, 6)
        pss = gwncppvgb.stop_wait_ack()
        dbg_data = blocks.message_debug() 
        dbg_ack = blocks.message_debug() 
        self.tb.msg_connect( (src, "out_port_0"), (pss, "in_port_0") )
        self.tb.msg_connect( (pss, "out_port_0"), (dbg_data, "print") )
        self.tb.msg_connect( (pss, "out_port_1"), (dbg_ack, "print") )

        self.tb.start ()
        time.sleep(8)
        self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_stop_wait_ack, "qa_stop_wait_ack.xml")

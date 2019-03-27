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

class qa_virtual_channel (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_constructor (self):
        #print "\n===\n=== TEST virtual_channel constructor \n===\n"
        #myblock = gwncppvgb.virtual_channel("GWN test message 1", 10)
        # set up fg
        #self.tb.run ()
        # check data
        pass

    def test_ports_timer (self):
        print "\n===\n=== TEST virtual_channel input and output ports \n===\n"
        tst_msg = "--- A message from message strobe"
        #src = blocks.message_strobe(pmt.intern(tst_msg), 1000)
        src = gwncppvgb.data_source("Data", "Data", "Data payload", 500, 12 )
        pss = gwncppvgb.virtual_channel(0.5)
        dbg = blocks.message_debug() 
        self.tb.msg_connect( (src, "out_port_0"), (pss, "in_port_0") )
        self.tb.msg_connect( (pss, "out_port_0"), (dbg, "print") )

        self.tb.start ()
        time.sleep(12)
        self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_virtual_channel, "qa_virtual_channel.xml")

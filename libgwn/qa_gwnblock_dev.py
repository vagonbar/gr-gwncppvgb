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

class qa_gwnblock_dev (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_constructor (self):
        #print "\n===\n=== TEST gwnblock_dev constructor \n===\n"
        #gwncppvgb.set_debug(True)   # does now work as expected
        #myblock = gwncppvgb.gwnblock_dev("GWN test message 1", 10)
        #print "myblock.__str__()", myblock.__str__()  # not as expected


        # set up fg
        #self.tb.run ()
        # check data
        pass

    def test_ports_timer (self):
        print "\n===\n=== TEST gwnblock_dev input and output ports \n===\n"
        tst_msg = "--- A message from message strobe"
        src = blocks.message_strobe(pmt.intern(tst_msg), 1000)
        pss = gwncppvgb.gwnblock_dev( \
          "TIMER 1 msg AAAA", 1000.0, 6)
          #"TIMER 1 msg AAAA", 1000.0, 6, \
          #"TIMER 2 msg BBBB", 1000.0, 3)
          #"TIMEOUT 2 CCC", 3000.0, 1)
        #"GWN test message 2", 10)

        dbg = blocks.message_debug() 
        self.tb.msg_connect( (src, "strobe"), (pss, "in_port_0") )
        self.tb.msg_connect( (pss, "out_port_0"), (dbg, "print") )
        #self.tb.msg_connect( (src, "port_out_0"), (dbg, "print") )
  

        self.tb.start ()
        time.sleep(6)
        self.tb.stop()


if __name__ == '__main__':
    gr_unittest.run(qa_gwnblock_dev, "qa_gwnblock_dev.xml")

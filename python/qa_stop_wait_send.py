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

class qa_stop_wait_send (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    #def test_constructor (self):
        #print "\n===\n=== TEST stop_wait_send constructor \n===\n"
        #gwncppvgb.set_debug(True)   # does now work as expected
        #myblock = gwncppvgb.stop_wait_send("GWN test message 1", 10)
        #print "myblock.__str__()", myblock.__str__()  # not as expected


        # set up fg
        #self.tb.run ()
        # check data
        #pass

    def test_fsm_debug (self):
        print "\n===\n=== TEST 1 FSM \n===\n"

        # data_source, ack block, data_sink
        blk_src = gwncppvgb.data_source("Data", "Data", "Data payload", 500.0, 10 )
        blk_ack = gwncppvgb.stop_wait_ack()
        #blk_dbg = blocks.message_debug() 
        blk_snk = gwncppvgb.data_sink() 

        ### Values for different tests
        # normal test: loss at 0.5, retries 3, buffer 3
        # retries test: loss at 0.8, retries 2, buffer 5
        # buffer test: loss at 0.8, retries 5, buffer 2

        # stop_wait_send(retries, timeout, buf_len, fsm_debug)
        blk_snd = gwncppvgb.stop_wait_send(3, 300.0, 3, False)  # change to True for debug
        # virtual channel 
        blk_vchan = gwncppvgb.virtual_channel(0.5)


        self.tb.msg_connect( (blk_src, "out_port_0"), (blk_snd, "in_port_0") )
        self.tb.msg_connect( (blk_snd, "out_port_0"), (blk_vchan, "in_port_0") )
        self.tb.msg_connect( (blk_vchan, "out_port_0"), (blk_ack, "in_port_0") )
        #self.tb.msg_connect( (blk_ack, "out_port_0"), (blk_dbg, "print") )
        self.tb.msg_connect( (blk_ack, "out_port_0"), (blk_snk, "in_port_0") )

        self.tb.msg_connect( (blk_ack, "out_port_1"), (blk_snd, "in_port_0") )

        self.tb.start ()
        time.sleep(20)
        self.tb.stop()
        return

if __name__ == '__main__':
    gr_unittest.run(qa_stop_wait_send, "qa_stop_wait_send.xml")

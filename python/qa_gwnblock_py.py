#!/usr/bin/env python
# -*- coding: utf-8 -*-
# 
# Copyright 2015-2019
#   Instituto de Ingenieria Electrica, Facultad de Ingenieria,
#   Universidad de la Republica, Uruguay.
# 
# This is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, version 3.
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
# 

from gnuradio import gr, gr_unittest
from gnuradio import blocks
from gwnblock_py import gwnblock_py

import pmt
import time
from gwnblock_py import mutex_prt



class qa_gwnblock_py (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    #def test_001_t (self):
    #    # set up fg
    #    self.tb.run ()
    #    # check data

    def test_gwnblock_py(self):
        '''GWN block in Python test.
        '''

        tst_msg = "--- A message from message strobe"
        src = blocks.message_strobe(pmt.intern(tst_msg), 1000)
        blk_gwn = gwnblock_py(name="MsgPasser", number_in=1, number_out=1, number_timers=0, number_timeouts=0)
        dbg = blocks.message_debug() 

        self.tb.msg_connect( 
            (src, "strobe"), 
            (blk_gwn, blk_gwn.ports_in[0].port) )
        self.tb.msg_connect( 
            (blk_gwn, blk_gwn.ports_out[0].port),
            (dbg, "print"))


        #self.tb.run()  # for flowgraphs that will stop on its own!
        self.tb.start() 
        #mutex_prt(self.tb.msg_edge_list())
        #print tb.dump()

        time.sleep(8)

        #blk_gwn.stop_timers()
        print '\n--- gwnblock_py, timers stopped'
        time.sleep(2)

        self.tb.stop()
        self.tb.wait()
        print '\n--- top block stopped'

        return

if __name__ == '__main__':
    gr_unittest.run(qa_gwnblock_py, "qa_gwnblock_py.xml")

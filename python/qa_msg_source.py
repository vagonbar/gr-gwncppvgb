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
from msg_source import msg_source

import pmt
import time
from gwnblock_py import mutex_prt


class qa_msg_source (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_msg_source(self):
        blk_src = msg_source(msg_count=4, interval=1.0)
        blk_src.timers[0].debug = False     # True
        #blk_src.timers[0].start()
        blk_dbg = blocks.message_debug()

        self.tb.msg_connect( 
            (blk_src, blk_src.ports_out[0].port), 
            (blk_dbg, "print") )

        #self.tb.run()  # for flowgraphs that will stop on its own!
        self.tb.start() 
        time.sleep(6)

        self.tb.stop()
        self.tb.wait()

        return


if __name__ == '__main__':
    gr_unittest.run(qa_msg_source, "qa_msg_source.xml")



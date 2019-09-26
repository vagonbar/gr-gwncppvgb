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
from <BLOCK_NAME> import <BLOCK_NAME>

import pmt
import time
from gwnblock_py import mutex_prt


class qa_<BLOCK_NAME> (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_<BLOCK_NAME>(self):
        #...
        #new_blk = <BLOCK_NAME>(<BLOCK_PARS>)
        #...

        #self.tb.msg_connect( 
        #    (<emitter_block>, <out_port), 
        #    (<receiver_block>, <in_port>) )


        #self.tb.run()  # for flowgraphs that will stop on its own!
        self.tb.start() 
        #mutex_prt(self.tb.msg_edge_list())
        #print tb.dump()

        #time.sleep(8)

        self.tb.stop()
        self.tb.wait()

        return


if __name__ == '__main__':
    gr_unittest.run(qa_<BLOCK_NAME>, "qa_<BLOCK_NAME>.xml")



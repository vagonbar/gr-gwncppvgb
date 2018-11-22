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
import gwncppvgb.gwncppvgb_swig as gwncppvgb

import time


class qa_gwntimer_strobe (gr_unittest.TestCase):

    def setUp (self):
        self.tb = gr.top_block ()

    def tearDown (self):
        self.tb = None

    def test_001_t (self):
        # set up fg
        #self.tb.run ()

        tmr = gwncppvgb.gwntimer_strobe ("Timer 1 msg", 1000.0, 9, "Timer 2 msg", 3000.0, 1)
        dst = blocks.message_debug()
        self.tb.msg_connect( (tmr, "strobe"), (dst, "print") )

        self.tb.start()
        time.sleep(11)
        #time.sleep(3)
        #tmr.timer_reset(2)
        #time.sleep(3)
        #tmr.timer_interrupt(2, True)
        #time.sleep(3)
        #tmr.timer_interrupt(2, False)
        #time.sleep(5)
        #tmr.timer_interrupt(1, True)
        #time.sleep(3)
        self.tb.stop()

        # check data


if __name__ == '__main__':
    gr_unittest.run(qa_gwntimer_strobe, "qa_gwntimer_strobe.xml")

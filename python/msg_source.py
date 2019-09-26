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

import numpy
from gnuradio import gr

# GWN imports
from gwnblock_py import gwnblock_py        # for all GWN blocks
from gwnblock_py import mutex_prt          # for tests


class msg_source(gwnblock_py):
    '''Emits a number of messages at regular intervals.

    '''
    def __init__(self, msg_count=10, interval=1.0):
      '''Message source constructor.

      @param msg_count: the number of messages to emit.
      @param msg_text: a string to include in message.
      '''
      gwnblock_py.__init__(self, name='msg_source', number_in=0, number_out=1, number_timers=1, number_timeouts=0)

      ### EXAMPLE CODE

      ## initialize timeouts, start
      #self.timeouts[0].timeout = 5
      #self.timeouts[0].start()
      
      ## initialize timers, start
      self.timers[0].retry = msg_count 
      self.timers[0].interval = interval
      #self.timers[0].exit_flag = False
      #self.timers[0].interrupt = False
      self.timers[0].msg_dc_1['Final'] = 'False'
      self.timers[0].msg_dc_2['Final'] = 'True'
      #self.timers[0].start()
      self.start_timers()

      return


    def process_data(self, py_msg):
        '''Sends received message from timer.

        @param py_msg: message, a Python data type.
        '''
        self.write_out(py_msg, port_nr=0)
        return

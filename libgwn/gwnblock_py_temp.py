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


class <BLOCK_NAME>(gwnblock_py):
    '''New GWN block in Python, created from template.

    '''
    def __init__(self, <BLOCK_PARS>):
      gwnblock_py.__init__(self, <GWNBLOCK_PARS>)

      ### EXAMPLE CODE

      ## initialize timeouts, start
      #self.timeouts[0].timeout = 5
      #self.timeouts[0].start()
      
      ## initialize timers, start
      #self.timers[0].retry = 5
      #self.timers[0].interval = 1.0
      #self.timers[0].start()
      #...

      return


    def process_data(self, py_msg):
        '''Sends received message, if timeout stops.

        @param py_msg: message, a Python data type.
        '''
        if type(py_msg) != str:    # TODO: change to use GWN data type dict
          self.write_out( ["--- TIMEOUT message", py_msg], port_nr=0)
          self.emit_msg = False
        else:
          if self.emit_msg:
            self.write_out(py_msg, port_nr=0)
        return

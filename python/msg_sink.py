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


class msg_sink(gwnblock_py):
    '''New GWN block in Python, created from template.

    '''
    def __init__(self):
      gwnblock_py.__init__(self, name='msg_sink', number_in=1, number_out=0, number_timers=0, number_timeouts=0)

      return


    def process_data(self, py_msg):
        '''Sends received message, if timeout stops.

        @param py_msg: message, a Python data type.
        '''
        print "*** Message received ***"
        print py_msg
        print "************************"
        return

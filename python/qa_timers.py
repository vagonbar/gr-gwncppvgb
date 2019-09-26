#!/usr/bin/env python
# -*- coding: utf-8 -*-
# test for timeout data in msg_dc


import time, sys
import gwnblock_py


def showtouts(timers):
    print '\n=== SHOW data in timers'
    for tout in timers:
        print 'Timer, port:', tout.port, '; port number:', tout.port_nr
        print '  msg_dc_1:', tout.msg_dc_1
    return


blk = gwnblock_py.gwnblock_py(name='TestTimers', number_timers=1)

print "=== test block data"
print "   block name:", blk.name()
print "   number of timers:", len(blk.timers)


blk.timers[0].retry = 5
blk.timers[0].interval = 1.0

# activate debug, load timeout name in msg_dc
for tout in blk.timers:
    tout.debug = True
    #tout.msg_dc_1['tout name'] = tout.port
    tout.msg_dc_1['Final'] = 'False'
    tout.msg_dc_2['Final'] = 'True'
showtouts(blk.timers)

print "\n=== START timers with different intervals and data in msg_dc"
blk.timers[0].start()
#blk.timeouts[1].start(timeout=2, msg_dc={'new_data':'bbb'})
#blk.timeouts[2].start(timeout=3, msg_dc={'new_data':'ccc'})

print "\n=== WAIT for events produced by timeout objects"
time.sleep(7)

print "\n=== CHANGE value in msg_dc only on timeout object 1"
#blk.timers[0].msg_dc_1={'new_key':'new key in timeout 1'})
blk.timers[0].stop()
blk.timers[0].msg_dc_1['new_key'] = 'new_key'
#blk.timers[0].start() # RuntimeError: "threads can only be started once" 

time.sleep(3)
showtouts(blk.timers)





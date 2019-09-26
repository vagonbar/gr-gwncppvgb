#!/usr/bin/env python
# -*- coding: utf-8 -*-
# test for timeout data in ev_dc


import time, sys
import gwnblock_py


def showtouts(timeouts):
    print '\n=== SHOW data in timeouts'
    for tout in timeouts:
        print 'Timeout, port:', tout.port, '; port number:', tout.port_nr
        print '  msg_dc:', tout.msg_dc
    return


blk = gwnblock_py.gwnblock_py(name='TestTimeouts', number_timeouts=3)

print "=== test block data"
print "   block name:", blk.name()
print "   number of timeouts:", len(blk.timeouts)


# activate debug, load timeout name in ev_dc
for tout in blk.timeouts:
    tout.debug = True
    tout.msg_dc['tout name'] = tout.port
showtouts(blk.timeouts)

print "\n=== START timeouts with different intervals and data in ev_dc"
blk.timeouts[0].start(msg_dc={'new_data':'aaa'})
blk.timeouts[1].start(timeout=2, msg_dc={'new_data':'bbb'})
blk.timeouts[2].start(timeout=3, msg_dc={'new_data':'ccc'})

print "\n=== WAIT for events produced by timeout objects"
time.sleep(5)

print "\n=== CHANGE value in ev_dc only on timeout object 1"
blk.timeouts[1].start(msg_dc={'new_key':'new key in timeout 1'})

time.sleep(3)
showtouts(blk.timeouts)




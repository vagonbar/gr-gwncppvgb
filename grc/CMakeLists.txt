# Copyright 2011 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
install(FILES
    gwncppvgb_symbol_strobe.xml
    gwncppvgb_message_timer_example.xml
    gwncppvgb_fsm_example.xml
    gwncppvgb_fsm_example_fsm.xml
    gwncppvgb_data_source.xml
    gwncppvgb_stop_wait_ack.xml
    gwncppvgb_virtual_channel.xml
    gwncppvgb_stop_wait_send.xml
    gwncppvgb_stop_wait_send_fsm.xml
    gwncppvgb_data_sink.xml
    gwncppvgb_gwnblock_py.xml
    gwncppvgb_msg_passer.xml
    gwncppvgb_msg_sink.xml
    gwncppvgb_msg_source.xml
    DESTINATION share/gnuradio/grc/blocks
)

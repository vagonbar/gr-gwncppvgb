# Copyright 2011,2012,2016 Free Software Foundation, Inc.
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

########################################################################
# Setup library
########################################################################
include(GrPlatform) #define LIB_SUFFIX

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})
list(APPEND gwncppvgb_sources
    symbol_strobe_impl.cc
    message_timer_example_impl.cc
    fsm_example_impl.cc
    fsm_example_fsm.cc
    data_source_impl.cc
    stop_wait_ack_impl.cc
    virtual_channel_impl.cc
    stop_wait_send_impl.cc
    stop_wait_send_fsm.cc
    data_sink_impl.cc )

set(gwncppvgb_sources "${gwncppvgb_sources}" PARENT_SCOPE)
if(NOT gwncppvgb_sources)
	MESSAGE(STATUS "No C++ sources... skipping lib/")
	return()
endif(NOT gwncppvgb_sources)

add_library(gnuradio-gwncppvgb SHARED ${gwncppvgb_sources})
target_link_libraries(gnuradio-gwncppvgb ${Boost_LIBRARIES} ${GNURADIO_ALL_LIBRARIES})
set_target_properties(gnuradio-gwncppvgb PROPERTIES DEFINE_SYMBOL "gnuradio_gwncppvgb_EXPORTS")

if(APPLE)
    set_target_properties(gnuradio-gwncppvgb PROPERTIES
        INSTALL_NAME_DIR "${CMAKE_INSTALL_PREFIX}/lib"
    )
endif(APPLE)

########################################################################
# Install built library files
########################################################################
include(GrMiscUtils)
GR_LIBRARY_FOO(gnuradio-gwncppvgb RUNTIME_COMPONENT "gwncppvgb_runtime" DEVEL_COMPONENT "gwncppvgb_devel")

########################################################################
# Build and register unit test
########################################################################
include(GrTest)

include_directories(${CPPUNIT_INCLUDE_DIRS})
list(APPEND test_gwncppvgb_sources
    ${CMAKE_CURRENT_SOURCE_DIR}/test_gwncppvgb.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/qa_gwncppvgb.cc
    )

add_executable(test-gwncppvgb ${test_gwncppvgb_sources})

target_link_libraries(
  test-gwncppvgb
  ${GNURADIO_RUNTIME_LIBRARIES}
  ${Boost_LIBRARIES}
  ${CPPUNIT_LIBRARIES}
  gnuradio-gwncppvgb
)

GR_ADD_TEST(test_gwncppvgb test-gwncppvgb)

########################################################################
# Print summary
########################################################################
message(STATUS "Using install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "Building for version: ${VERSION} / ${LIBVER}")


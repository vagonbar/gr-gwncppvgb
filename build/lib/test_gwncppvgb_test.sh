#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/victor/IIE/GNURadio/gr-gwncppvgb/lib
export PATH=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/lib:$PATH
export LD_LIBRARY_PATH=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-gwncppvgb 

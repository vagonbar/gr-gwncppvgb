#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/victor/IIE/GNURadio/gr-gwncppvgb/python
export PATH=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/python:$PATH
export LD_LIBRARY_PATH=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=/home/victor/IIE/GNURadio/gr-gwncppvgb/build/swig:$PYTHONPATH
/usr/bin/python2 /home/victor/IIE/GNURadio/gr-gwncppvgb/python/qa_gwnblock.py 

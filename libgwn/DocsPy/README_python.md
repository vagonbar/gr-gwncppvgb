# GWN, Python version

GWN, the GNU Wireless Network, is a network development toolkit compatible with GNU Radio.

This version is the GWN project (GWN, GNU Wireless Network) coded in Python.


## Example blocks
  To be written.
  Please see `examples/msg_passer_py_example.grc`

### Timers example block
  To be written.

### FSM example block.
  To be written.


## Creation of a new block

A new block can be easily created with the script `gwn_modtool_py.py`, similar to the GNU Radio `gr_modtool`. This script must be run from the `build` directory:
```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool_py 
```

Script user help:

GWN new block creation script.
This script must be executed from the 'build' directory.
The following positional parameters may be given:
    <new_block_name> <nr_in> <nr_out> <nr_timers> <nr_timeouts>
These indicate the new block name, and the numbers of input ports, output ports, timers and timeouts.
If invoked with no positional parameters, the script will ask for them.
These parameters define the new block construction; they are not visible to block users. 
Next, the script will ask for the new block's own parameters, a list of Python parameters such as 
    msg_count, interval=100, payload="an example payload"
After confirmation, the script will create the new block with all parameters included for the programmer to customize.




On execution, the user will be asked to indicate the characteristics of the new block:
```
    Block_name: 
    Number of input ports:
    Number of output ports: 
    Number of timers:
    Number of timeouts:
    New block parameter list: <Python format parameter list>
```


TODO: The two example blocks can be cloned to run immediately under the block name chosen by the user. This allows to start with a running block which can then be altered to reflect specific needs. To clone the example blocks, please see the following documents:



[Back to README](../../README.md)

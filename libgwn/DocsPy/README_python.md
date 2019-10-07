# GWN, Python version

GWN, the GNU Wireless Network, is a network development toolkit compatible with GNU Radio.

This version is the GWN project (GWN, GNU Wireless Network) coded in Python.

## Example blocks
  Please see `examples/msg_passer_py_example.grc`

### FSM example block.
  FSM in Python not yet available.

## Creation of a new block

A new block can be easily created with the script `gwn_modtool_py.py`, similar to the GNU Radio `gr_modtool`. This script must be run from the `build` directory:
```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool_py.py 
```

Example invocation code with optional parameters:
```
  cd my_project/build
  ../libgwn/gwn_modtool_py.py my_new_block 1 1 0 0
```
Creates a new block with 1 input port, 1 output port, 0 timers, 0 timeouts. Alternatively, the script may be invoked with no optional parameters, and they will be asked by the script.

The script must be executed from the 'build' directory.
The following optional positional parameters may be given:
```
  <new_block_name> <nr_in> <nr_out> <nr_timers> <nr_timeouts>
```
These indicate the new block name, and the numbers of input ports, output ports, timers and timeouts.
If invoked with no positional parameters, the script will ask for them.
These parameters define the new block construction; they are not visible to block users. 
Next, the script will ask for the new block's own parameters, a list of Python parameters such as
```
    msg_count, interval=100, payload="an example payload"
```
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

[Back to README](../../README.md)
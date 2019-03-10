# GWN : GNU Wireless Network

This is a testing project to enhance the GWN project (GWN, GNU Wireless Network) by coding the core of GWN in C++ (at present it is coded in Python). Please find the GWN Project at https://github.com/vagonbar/gr-gwn/.

## Purpose, difficulties, workarounds

The coding in C++ of the core classes in GWN is expected to improve in performance and become closer to the general design of GNU Radio. Since GWN is implemented as an OOT (Out Of Tree) module in GNU Radio, and GNU Radio does not allow inheritance from user blocks, but only from GNU Radio blocks, the implementation in C++ of the core classes of GWN could not follow the straightforward way of inheriting from a GWN basic block, called gwnblock in GWN. This restriction was not found in Python coded blocks, where inheritance from gwnblock did not show any restrictions. Hence, in a new C++ block, all GWN facilities must be included in the new block. 

To simplify coding of new blocks, a block creation script was written, which includes all necessary code in a new GWN block by modifying a template to include the new block ports, timers and user parameters. The programmer of the new block is expected to rewrite two functions:

`add_init`, which stands for additional initialization, where custom variables can be defined, timers can be set, etc.

`process_data`, where all the actions expected from the new block are coded. This function is invoked when a message is received on a message or timer port, does some processing according to the message received, and optionally outputs another message on any of the available output ports.


## Example blocks

The main funcionalities of GWN can be appreciated through two example blocks with their corresponding QA tests and GRC flowgraphs. One of them illustrates the use of timers, and the other the inclusion of a Finite State Machine (FSM).

### Timers example block

An example block called `message_timer_example` is already created, and can be tested by running its corresponding QA test, `python/qa_message_timer_example.py`. 

This block receives a message on an input port and outputs the same message on an output port, as well as messages produced by two internal timers, also sent through the output port. In this flowgraph:
    `message_strobe --> message_timer_example --> message_debug`
The `message_debug` block also receives timer messages generated in the block's internal timer.

### FSM example block.

An example block called `fsm_test` is already created, and can be tested by runningits corresponding QA test `python ../qa_fsm_example.py`.

This block receives a message on its input port, passes it to its associated FSM engine for it to execute a transition, and outps the result produced by the FSM on its output port. In this flowgraph:
    `symbol_strobe --> fsm_example --> message_debug`
The `message_debug` block receives the result of the FSM action, plus some information on symbol received and current state of the FSM engine. At the end, the contents of the FSM associated memory is displayed.

## Creation of a new block

A new block can be easily created with the script `gwn_modtool.sh`, similar to the GNU Radio `gr_modtool`. This script must be run from the `build` with the following syntax:
    ```
    cd <project_root_directory>/build
    ../libgwn/gwn_modtool <block_name> <nr_inputs> <nr_outputs> <nr_timers
    ```
All parameters must be given; any of them can be 0. In the course of the script run, the user is asked to provide a list of parameters in C++ format, such as `std::string message, int count, bool debug`; this list may be empty. The user will then be asked if an FSM engine is to be created and associated to the main block. The script creates the blocks with the names, inputs, outputs, timers, and parameters given, and an associated block with the FSM engine if one has been asked for.

Once the blocks are created, the user is responsible for reflecting in the code her selections of ports and parameters. To this purpose, the corresponding sections in the code are identified for the user to alter.

The two example blocks can be cloned to run immediately under the block name chosen by the user. This allows to start with a running block which can then be altered to reflect specific needs. To clone the example blocks, pleas see the following documents:

[The gwn_modtool.sh script](libgwn/Docs/GWN_modtool.md)

[Cloning of the timer example block](ibgwn/Docs/Block_example.md)

[Cloning ot the FSM example block](libgwn/Docs/FSM_example.md)

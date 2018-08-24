# GWN Cpp Vgb

This is a testing project to enhance the GWN project (GWN, GNU Wireless Network) by coding the core of GWN in C++ (at present it is coded in Python). Please see the GWN Project at https://github.com/vagonbar/gr-gwn/

## Purpose, difficulties, workarounds

The coding in C++ of the core classes in GWN is expected to improve in performance and become closer to the general designe of GNU Radio. Since GWN is implemented as an OOT (Out Of Tree) module in GNU Radio, and GNU Radio does not allow inheritance from user blocks, but only from GNU Radio blocks, the implementation in C++ of the core classes of GWN could not follow the straightforward way of inheriting from a GWN basic block, called gwnblock in GWN. Hence, in a new block, all GWN facilities must be included in the new block. 

To simplify coding of new blocks, a block creation script was written, which produces two blocks: a first one with all necessary GWN code, and a second block with a function named process_data. This last function should be the only one to be modified in a new type of GWN block: it receives a message from the input ports of the new block, the programmer does some processing with this message, and optionally outputs another message on one or several of the output ports of the block.

## Creation of a new block

Say we want to create a new block called `message_repeat`, which receives a message on an input port and outputs the same message, eventually modified, on an output port. The commands to create this new block would be:


```
  cd <project_root_directory>/build
  ../apps/gwn_modtool.sh message_repeat 1 1 0 0 
```

`gwn_modtool` is a script which uses gr_modtool to create two blocks:

- message_repeat, which inherits from GNU Radio general block, where all the GWN facilieties are coded.
- message_repeat_pdata, a block of type `noblock`, where the function process_data resides.  

The script takes its code from gwnblock, a template GWN block, and modifies names and parameter lists according to the parameters given to `gwn_modtool`: block name, number of input ports, number of output ports, number of timers, number of timeouts (the last two not yet implemented, so 0 is required).

Once these blocks have been created, the programmer has at her disposal the necessary input and output message ports, and she only needs to customize the function process_data, found in block <name_of_new_block>_pdata.

## A new block creation test

The following commands create a message_repeat block, using a process_data function coded in the template class gwnblock. The block contains an input and an output ports, and repeats the message received in the input port by sending it on the output port, in this flowgraph:
    message_strobe --> message_repeat --> message_debug
This test is included in the qa_message_repeat.py QA test code.

The complete code to create and test the new block follows:

```
  cd <project_root_directory>/build
  ../apps/gwn_modtool.sh message_repeat 1 1 0 0 
  python ../python/qa_message_repeat.py
```

An output similar to the following is expected:

```
===
=== TEST message_repeat input and output ports 
===

message_repeat, constructor, name block_2, number_in 1, number_out 1
GWNPort, default constructor
GWNOutPort, constructor
...about to register out_port
...registered out_port
=== message_repeat, out ports:
  out port 0: GWNPort name: out_port_0, number: 0, in block: block_2
GWNPort, default constructor
GWNInPort, constructor
...about to register in_port
...registered in_port
=== message_repeat, in ports:
  in port 0: GWNPort name: in_port_0, number: 0, in block: block_2
=== message_repeat, receive and send test
Handle msg, received: --- A test message from message strobe
...message_repeat_data, event received: --- A test message from message strobe
Post message, sent: ...message_repeat, event processed: --- A test message from message strobe
******* MESSAGE DEBUG PRINT ********
...message_repeat, event processed: --- A test message from message strobe
************************************
```

In the general case, a different number of input and output ports may be given, and proper customization of the process_data function expected, to accomplish the desired processing of messages received into messages sent.


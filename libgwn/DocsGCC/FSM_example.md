# GWN : An example GWN block with an FSM engine

The GWN project offers the possibility of integrating an FSM (Finite Sate Machine) to a GWN block. This document describes the creation (cloning) of a block similar to the FSM block example.

A descriptions of the GWN FSM as autonomous code can be found in the [gwnfsm](https://github.com/vagonbar/gwnfsm) project.

## Example FSM engine

An example FSM engine is included to help developers implement their own FSM engines. The example FSM implements the following diagram:

![FSM example](../Images/gwnfsm_test.png)

The example FSM symbols are characters. In the flowgraph

    `symbol_strobe --> fsm_example --> message_debug`

the example block `symbol_strobe` receives a string as a parameter and emits one character at a time. The example flowgraph `grc/symbol_strobe_test.grc` shows how it works.

## A new FSM block creation

An optional FSM (Finite State Machine) block can be created. The FSM is included in its own block, which can be associated to the new block under creation. The FSM is instanciated as a class accessible from the new block through a pointer in the new block. The FSM is ordered to execute a transition through its function `process(input_symbol, message_to_function, "")`. This FSM function is typically invoked in the `process_data()` function of the new block to which the FSM is associated. The symbol may be any type of data, according to the way the FSM is coded. When receiving the symbol, the FSM machine looks for a transaction which includes the received symbol, the current state, and a condition which must evaluate to true. If such a transaction is found, an action function is executed, and a new state is made the current state of the FSM. The action function may produce a result which is returned to the caller, i.e. the `process_data()` function in the new block.

The following commands clone the `fsm_example` block described in the [README page](../../README.md). To account for the parameteres needed for the example to run as created, the user argument list must be:

  `bool fsm_debug`

These parameters allow to test the block including optional debug messages. The complete sequence of commands to create a block named `my_fsm_example` follows.

```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool.sh my_fsm_example 1 1 0
  (answer questions:
     enter argument list,
     confirm creation say "y",
     optional FSM block say "y",
     add C++ QA code as preferred)
  make
  make install
  python ../python/qa_my_fsm_example.py
```

This is for a GNU Radio installation in user space, as described in 
 `https://github.com/gnuradio/pybombs`.
For a system wide GNU Radio installation, instead of `make install` you must write `sudo make install`.

After creation, if you gave other names to parameters, you must alter the timer initialization in the `process_data` function, and also in the QA Python test. 

### FSM block QA testing

After running the QA test a result similar to the following is expected:

```
===
=== TEST 1 FSM with debug 
===

...FSM variables: where=A, to_c=0. 
...FSM memory show and erase, INIT state
   Memory size: 0, contents:

******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: ,  current state INIT

...FSM state: input symbol: s,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: s,  current state INIT
Result of function fn_goA. 
...FSM state: input symbol: g,  current state STATE_A

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: g,  current state STATE_A
Result of function fn_none. 
...FSM state: input symbol: x,  current state STATE_A

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: x,  current state STATE_A
Result of function fn_none. 
...FSM state: input symbol: y,  current state STATE_A

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: y,  current state STATE_A
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of fn_chgwhr, where=B. 
...FSM state: input symbol: w,  current state CHG_WHERE

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: w,  current state CHG_WHERE
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of function fn_goB. 
...FSM state: input symbol: g,  current state STATE_B

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: g,  current state STATE_B
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of fn_chgwhr, where=C. 
...FSM state: input symbol: w,  current state CHG_WHERE

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: w,  current state CHG_WHERE
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of fn_toC, to_c=1. 
...FSM state: input symbol: c,  current state CHG_TOC

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: c,  current state CHG_TOC
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of function fn_goA. Result of function fn_goB. 
...FSM state: input symbol: g,  current state STATE_C

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: g,  current state STATE_C
Result of function fn_init. 
...FSM state: input symbol: r,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: r,  current state INIT
Result of function fn_none. 
...FSM state: input symbol: x,  current state INIT

************************************
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: x,  current state INIT
Result of function fn_none. 
...FSM state: input symbol: y,  current state INIT

************************************
...FSM variables: where=C, to_c=1. 
...FSM memory show and erase, INIT state
   Memory size: 5, contents:
 : C visited!  : B visited! : A visited! : B visited! : A visited!
******* MESSAGE DEBUG PRINT ********
...FSM state: input symbol: y,  current state INIT

...FSM state: input symbol: s,  current state INIT

************************************
.
----------------------------------------------------------------------
Ran 1 test in 20.008s

OK
```

## FSM block customization

Please see the following example files for a block with an associated FSM block:
* example block: files `include/gwncppvgb/fsm_test.h, lib/fsm_test_impl.h, lib/fsm_test_impl.cc`. This is the example block to which the FSM block is associated.
* FSM associated block: `include/gwncppvgb/fsmblk.h, lib/fsmblk.cc`. This is the associated block which implements the FSM machine. Transitions, action functions, function conditions and custom variables which account for a specific FSM are included in these files.
* QA test code: file `python/qa_fsm_test_example.py`.
* GRC flowgraph: file `examples/fsm_test_example.grc.`

To customize the FSM block, the following sections must be rewritten:

In the `include/gwncppvgb/<fsm_block>.h` file:

`// user defined variables and functions` 

In the `lib/<fsm_block>.cc` file :

`// user defined variables and functions`

`// condition function definitions`

`// other user defined functions`

`// user transitions`

In the new block to which the FSM is associated, these sections must be rewritten:

`/* Timer and input messages processing, REWRITE as desired. */`

`// invoke FSM with symbol`

`/* Additional initialization, REWRITE as desired. */`


[Back to README GCC](README_cc.md)

[Back to README](../../README.md)


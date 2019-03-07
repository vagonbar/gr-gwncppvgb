# GWN Cpp Vgb - The FSM Engine

The GWN project offers the possibility of integrating an FSM (Finite Sate Machine) to a GWN block.

A descriptions of the GWN FSM as autonomous code can be found in the [gwnfsm](https://github.com/vagonbar/gwnfsm) project.

## Example FSM engine

An example FSM engine is included to help developers implement their own FSM engines. The example FSM implements the following diagram:

![FSM example](../Images/gwnfsm_test.png)


After running the QA test
`python ./python/qa_fsm_test.py`
a result similar to the following is expected:

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

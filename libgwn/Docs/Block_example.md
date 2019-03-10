# GNU : An examplo GWN block with timers

This document describes the creation (cloning) of a block similar to the timer message block example.

## A new timer block creation

The following commands clone the `message_timer_repeat` block described in the [README page](../README-md). To account for the parameteres needed for the example to run as created, the user argument list must be:

  `std::string msg_1, float period_1, int count_1, std::string msg_2, float period_2, int count_2`

These parameters indicate the message, period, and quantity (count) of messages emitted by the two internal timers. The complete sequence of commands to create a block named `my_timer_example` follows.


```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool.sh my_timer_example 1 1 2
  (answer questions: enter argument list, confirm creation say "y", optional FSM block say "n", add C++ QA code as preferred)
  make
  make install
  python ../python/qa_my_timer_example.py
```

This is for a GNU Radio installation in user space, as described in 
 `https://github.com/gnuradio/pybombs`.
For a system wide GNU Radio installation, instead of `make install` you must write `sudo make install`.

After creation, if you gave other names to parameters, you must alter the timer initialization in the `process_data` function, and also in the QA Python test. 

### Timer block QA testing

When running the QA test, an output similar to the following is expected:

```
===
=== TEST message_timer_example input and output ports 
===

message_timer_example, constructor, name message_timer_example, number_in 1, number_out 1
=== message_timer_example, out ports:
  out port 0: GWNPort name: out_port_0, number: 0, in block: message_timer_example
=== message_timer_example, in ports:
  in port 0: GWNPort name: in_port_0, number: 0, in block: message_timer_example
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 1) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 2 msg BBBB) (seq_nr . 1) (subtype . timer_1) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 2) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 2 msg BBBB) (seq_nr . 2) (subtype . timer_1) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 3) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 2 msg BBBB) (seq_nr . 3) (subtype . timer_1) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 4) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 5) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
******* MESSAGE DEBUG PRINT ********
((message . TIMER 1 msg AAAA) (seq_nr . 6) (subtype . timer_0) (type . Timer))
************************************
******* MESSAGE DEBUG PRINT ********
--- A message from message strobe
************************************
terminate called after throwing an instance of 'boost::thread_interrupted'
```

In the general case, a different number of input and output ports may be given, and proper customization of the `add_init` and `process_data` functions is required.


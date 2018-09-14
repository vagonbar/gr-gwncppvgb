# GWN Cpp Vgb

This is a testing project to enhance the GWN project (GWN, GNU Wireless Network) by coding the core of GWN in C++ (at present it is coded in Python). Please find the GWN Project at https://github.com/vagonbar/gr-gwn/.

## Purpose, difficulties, workarounds

The coding in C++ of the core classes in GWN is expected to improve in performance and become closer to the general design of GNU Radio. Since GWN is implemented as an OOT (Out Of Tree) module in GNU Radio, and GNU Radio does not allow inheritance from user blocks, but only from GNU Radio blocks, the implementation in C++ of the core classes of GWN could not follow the straightforward way of inheriting from a GWN basic block, called gwnblock in GWN. This restriction was not found in Python coded blocks, where inheritance from gwnblock did not show any restrictions. Hence, in a new C++ block, all GWN facilities must be included in the new block. 

To simplify coding of new blocks, a block creation script was written, which produces two blocks: a first one with all necessary GWN code, and a second block with a function named `process_data`. This last function should be the only one to be modified in a new type of GWN block: it receives a message from the input ports of the new block, the programmer does some processing with this message, and optionally outputs another message on one or several of the output ports of the block.

## Creation of a new block

Say we want to create a new block called `message_repeat`, which receives a message on an input port and outputs the same message, eventually modified, on an output port. The commands to create this new block would be:


```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool.sh message_repeat 1 1 0 
```

The script `gwn_modtool.sh` takes the code from `gwnblockc`, a template GWN block available in the `../libgwn` directory, and modifies the argument list according to the argumets given to `gwn_modtool.sh` as positional parameters: block name, number of input ports, number of output ports, and number of timers (the last one not yet implemented, so 0 is required).

When running the script, the programmer will be asked to enter a user argument list of its own, such as `std::string message, int counter`; this list may be empty. 

`gwn_modtool.sh` is a bash script which uses GNU RAdio `gr_modtool` to create two blocks:

- `message_repeat`, which inherits from GNU Radio general `gr::block`; all the GWN facilities for communicating with other blocks are copied into this block from a template GWN block. In most cases, the programmer needs not change anything in this block.

- `message_repeat_pdata`, an auxiliary block of GNU Radio type `noblock`, where the function `process_data` resides, and where local attributes are defined to capture user parameters. More specifically, for an argument called `arg_name` in the user list, the script declares an attribute `d_arg_name` of the corresponding type, and inserts an assignment `d_arg_name = arg_name` to capture the argument value (parameter) in the attribute variable. The files which the programmer must modify to achieve her purposes are these two: `../include/<module_name>/message_repeat_pdata.h`, and `../lib/message_repeat_pdata.cc`.

TODO: implement timers.

## A new block creation test

The following commands create a `message_repeat` block. This block contains an input and an output ports, and repeats the message received in the input port by sending it on the output port, in this flowgraph:

    `message_strobe --> message_repeat --> message_debug`

This test will be included in the `qa_message_repeat.py` QA test code.

The new block can be created with the following commands:

```
  cd <project_root_directory>/build
  ../libgwn/gwn_modtool.sh message_repeat 1 1 0
  (answer questions: argument list, confirm creation, add C++ QA code)
  make
  make install
  python ../python/qa_message_repeat.py
```

This is for a GNU Radio installation in user space, as described in 
 `https://github.com/gnuradio/pybombs`.
For a system wide GNU Radio installation, instead of `make install` you must write `sudo make install`. 

## An example block

An example block is provided, which was created with the former commands,  inserting this argument list:

```
  std::string message, int counter
```

In file `lib/message_passer_pdata.cc` you will find the constructor and destructor of the class, with the following statements:

```
  message_passer_pdata::message_passer_pdata(std::string message, int counter)
  {
    // GWN user arguments initialization
    d_message = message;
    d_counter = counter;
  }
  message_passer_pdata::~message_passer_pdata() { }
```

As you can see, the user arguments have been inserted as the signature of the constructor, and the corresponding attributes initialized. Declarations for these functions and attributes can be found in `include/<module_name>/message_passer_pdata.h`:

```
  class GWNCPPVGB_API message_passer_pdata
  {
  public:
    message_passer_pdata(std::string message, int counter);
    ~message_passer_pdata();
    pmt::pmt_t process_data(std::string ev);
  private:
    // GWN user arguments declaration
    std::string d_message;
    int d_counter;
  };
```

Declaration of function `process_data` also appears in this file. For the present example block, the `process_data` function has been changed to include in the output message the user parameters indicated:

```
  pmt::pmt_t message_passer_pdata::process_data(std::string ev)
  {
    std::string ev_proc = "...process_data, received: " +
      ev + "\n   User parameter message: " + d_message + 
      "\n   User parameter counter: " + to_string(d_counter) +
      "\n"; 
    d_counter = d_counter + 1;
    pmt::pmt_t pmt_port = pmt::string_to_symbol("out_port_0");
    pmt::pmt_t pmt_msg = pmt::string_to_symbol(ev_proc); 
    return pmt::cons(pmt_port, pmt_msg);
  }
```

An output similar to the following is expected:

```
Handle msg, received: --- A test message from message strobe
Post message, sent: <...process_data, received: --- A test message from message strobe
   User parameter message: GWN test message 2
   User parameter counter: 13
> on port: out_port_0
******* MESSAGE DEBUG PRINT ********
...process_data, received: --- A test message from message strobe
   User parameter message: GWN test message 2
   User parameter counter: 13

************************************
Handle msg, received: --- A test message from message strobe
Post message, sent: <...process_data, received: --- A test message from message strobe
   User parameter message: GWN test message 2
   User parameter counter: 14
> on port: out_port_0
******* MESSAGE DEBUG PRINT ********
...process_data, received: --- A test message from message strobe
   User parameter message: GWN test message 2
   User parameter counter: 14

************************************
```

In the general case, a different number of input and output ports may be given, and proper customization of the `process_data` function is expected, to accomplish the desired processing of messages received into messages sent.

TEMPORARY: in the former output, please note several debug messages can be seen; this is controlled by an attribute `d_debug` in the template class `gwnblockc`. This is set to `true` at this stage of development, when this project reaches production it will be set to `false` so as to omit debug messages.





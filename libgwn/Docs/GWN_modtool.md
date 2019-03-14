# GNU : The `gwn_modtool.sh` script

The script `gwn_modtool.sh` takes the code from a template GWN block available in the `../libgwn` directory, and modifies the argument list according to the argumets given to `gwn_modtool.sh` as positional parameters: block name, number of input ports, number of output ports, and number of timers.

When running the script, the programmer will be asked to enter a user argument list of his own, such as `std::string message, float period, int count`; this list may be empty. 

`gwn_modtool.sh` is a bash script which uses GNU Radio `gr_modtool` to create the new block, which inherits from GNU Radio general `gr::block`; all the GWN facilities for communicating with other blocks are copied into the new block from the template GWN block. Once created, the programmer will find clearly indicated in the code the functions to rewrite, i.e. `add_init()` and `process_data()`. The new block includes local attributes to capture the user parameters. More specifically, for an argument called `arg_name` in the user parameters list, the script declares an attribute `d_arg_name` of the corresponding type, and inserts an assignment `d_arg_name(arg_name)` in the constructor to capture the argument value (parameter) in the attribute variable.

When creating a new block, an optional FSM (Finite State Machine) can be attached to the block. To this purpose, the script `gwn_modtool.sh` asks whether to create the FSM associated block or not. If the answer is `y`, the FSM block is created.

Please see the documents referred to in the [README page](../../README.md) to clone the example blocks or create your own. It is strongly suggested to start cloning the example blocks, since this allows to start with a running block which can then be customized to specific needs.

[Back to README](../../README.md)


# GWN : GNU Wireless Network

GWN, the GNU Wireless Network, is a network development toolkit compatible with GNU Radio.

The GNU Wireless Network project (GWN) is a toolkit to allow teaching and experimentation in wireless data networks. It is conceived as an out-of-tree module of GNU Radio, a popular Software Defined Radio (SDR) implementation. GWN is based on the tagged streams and messages of GNU Radio which allow asynchronous item transfer among blocks, and message passing to blocks upstream in the flowgraph. GWN adds some other features essential to data network protocol implementation, such as timers on each block, and access to a finite state machine. GWN provides a template block class which implements a variable number of inputs, outputs, and inner timers. An additional template block with a Finite State Machine (FSM) can be used to create a complementary block and associate it to the new block. This allows for an easy way to extend GWN with new blocks apt for data network communications. Besides these templates to create new blocks, the GWN toolkit comprises some blocks which implement atomic functions frequently used in data network protocol implementation. The toolkit was tested in real world communication among computers using USRP hardware devices.


## GWN messages and ports

Each GWN block may contain 0, 1 or more input ports, and 0, 1 or more output ports. Input and output ports are message ports. A message is a GNU Radio PMT ([PolyMorphic data Type](https://www.gnuradio.org/doc/doxygen/page_pmt.html)). GNU Radio provides a very complete set of functions to convert to and from a number of data types into PMT data types.

Messages are just a PMT dictionary type. This allows the user to insert any data type identified by a name. GWN suggests to use Type and Subtype as keys in the message dictionary, since these pieces of information are commonly used in data networks. However, the user may interchange between her blocks messages of any PMT type.


## GWN Timers

A GWN timer is defined in a class nested in the GWN block. For each timer used in the block, a new instance of this class is created and associated with the GWN block. Each timer emits internal messages periodically for a number of times. A timer can be given as parameters the message to emit, the period of time between messages, and the total number of messages to emit.
 
The block containing the timer receives messages from the timer in an internal input port. This internal port, called a timer port, is a message port of the block, but it is not connected to any external block. All timers in the block send their messages to this unique timer input port.
When started, the timer waits for the indicated period of time before emitting its first message. Then, the timer goes on sending messages after each period of time, until reaching the number of messages indicated as a count parameter. Once the count has been reached, no more messages are sent. However, the timer thread is not finished, and can be restarted.
The timer can be suspended in its emission of messages. When suspended, the timer does not emit messages, and the counter is not incremented, but the timer thread remains alive. When taken out from suspension, messages continue to be emitted and the counter is incremented from its last value.
The counter can be resetted, thus starting to emit messages as if it was recently started.
The timer can be stopped before reaching its assigned number of messages to emit. In this case, no messages are emitted any more, and the timer thread is terminated.
The message emmited by the timer is a GWN message, i.e. a port identifier and a dictionary in PMT format. In a GWN message, the dictionary contains a type, a subtype, and a sequence number, with the optional addition of other entries defined by the user. Each message is passed to the main block function `process_data`, where the actions defined by the programmer happen.


## Framework in Python or C++?

The project was originally coded in Python. When it was reasonably stable and several applications were successfully tested, an attempt was made to rebuild the project core in C++. Ideally this would allow to inherit from a generic GWN block in Python or C++. This could not be: the structure of GNU Radio does not allow for second time inheritance: the generic GWN block inherits from GNU Radio `basic_block`, and a new block GWN block is expected to inherit from the generic GWN block. This is so in Python, but is not allowed in C++. The only way to achieve this second time inheritance is to code the generic GWN block in Python, and descendans can only be coded in Python. In other words: 
- if the generic GWN block is coded in Python, a user block can be written in Python and can inherit from the generic GWN block. Successive inheritance is also possible.
- if the generic GWN block is coded in C++, a user block cannot inherit from the generic GWN block.

In C++, a workaround was to avoid inheritance and use the generic GWN block as a template, thus including all necessary code for ports, timers and FSM in any new block, clearly indicating the sections where the programmer was to write his own code. This was an obviously cumbersome solution, but it worked. Some example blocks and a simple Stop and Wait network protocol application were successfully tested. Since most programmers interested in GNU Radio applied to data networks would be experimenting and prototyping, GWN in C++ may easily discourage its use, the development team considered a return to Python. It was also decided to keep the C++ version, in case it would be of interest for some particular project, or more hopefully to find a practical way to circumvent GNU Radio C++ limitations and allow for normal C++ inheritance. 

Since development in C++ or Python are intrinsecally different, documentation has been written for both situations.

[GWN in Python](libgwn/DocsPy/README_python.md)

[GWN in C++](libgwn/DocsGCC/README_cc.md)


## Publications
* _GWN : A framework for packet radio and medium access control in GNU radio_. Víctor González Barbone, Pablo Belzarena, Federico Larroca, Martín Randall, Paola Romero, Mariana Gelós. Wireless Innovation Forum Conference on Wireless Communications Technologies and Software Defined Radio (WInnComm 17), San Diego, CA, USA, 13-17 nov, page 1--10 - 2017 [PDF](https://iie.fing.edu.uy/publicaciones/2017/GBLRRG17/GBLRRG17.pdf)

### Acknowledgements

This project was partially funded by program CSIC-Grupos, Universidad de la Republica (UdelaR), Uruguay.


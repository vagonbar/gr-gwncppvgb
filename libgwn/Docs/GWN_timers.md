# GWN Timers

A GWN timer is defined in a class nested in the GWN block. For each timer used in the block, a new instance of this class is created and associated with the GWN block. Each timer emits internal messages periodically for a number of times. A timer can be given as parameters the message to emit, the period of time between messages, and the total number of messages to emit.
 
The block containing the timer receives messages from the timer in an internal input port. This internal port, called a timer port, is a message port of the block, but it is not connected to any external block. All timers in the block send their messages to this unique timer input port.
When started, the timer waits for the indicated period of time before emitting its first message. Then, the timer goes on sending messages after each period of time, until reaching the number of messages indicated as a count parameter. Once the count has been reached, no more messages are sent. However, the timer thread is not finished, and can be restarted.
The timer can be suspended in its emission of messages. When suspended, the timer does not emit messages, and the counter is not incremented, but the timer thread remains alive. When taken out from suspension, messages continue to be emitted and the counter is incremented from its last value.
The counter can be resetted, thus starting to emit messages as if it was recently started.
The timer can be stopped before reaching its assigned number of messages to emit. In this case, no messages are emitted any more, and the timer thread is terminated.
The message emmited by the timer is a GWN message, i.e. a port identifier and a dictionary in PMT format. In a GWN message, the dictionary contains a type, a subtype, and a sequence number, with the optional addition of other entries defined by the user. Each message is passed to the main block function `process_data`, where the actions defined by the programmer happen.


[Back to README](../../README.md)

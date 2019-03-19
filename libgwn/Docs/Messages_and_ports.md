# GWN messages and ports

Each GWN block may contain 0, 1 or more input ports, and 0, 1 or more output ports. Input and output ports are message ports. A message is a GNU Radio PMT ([PolyMorphic data Type](https://www.gnuradio.org/doc/doxygen/page_pmt.html)). GNU Radio provides a very complete set of functions to convert to and from a number of data types into PMT data types.

Messages are just a PMT dictionary type. This allows the user to insert any data type identified by a name. GWN suggests to use Type and Subtype as keys in the message dictionary, since these pieces of information are commonly used in data networks. However, the user may interchange between her blocks messages of any PMT type.

[Back to README](../../README.md)

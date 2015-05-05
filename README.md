clib
====

This library provided:

* A serialized hash map
	* write/load a serialized hash-map.
* A logger
	* independent, depends on nothing(not like log4cxx).
	* supporting daily logrotate(not like google log [glog])
	* supporting multi-thread write
	* supporting debug/release compiling(erase all debug functions).
	* Unix/Posix/Max OS supported.

require:
	GCC > 4.1









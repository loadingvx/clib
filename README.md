clib
====

This library provides:

* A serialized hash map
	* Write/load a serialized hash-map.

* A logger
	* Independent, depends on nothing(not like log4cxx).
	* Supporting daily logrotate(not like google log [glog])
	* Supporting multi-thread write
	* Supporting debug/release compiling(erase all debug functions).
	* Unix/Posix/Max OS supported.

* A levenshtein distance(edit distance) calculator.
	* Action costs ( del:insert:replace = 1:1:1 ) was defined to be equal.
	* Change the costs as you wish.
	* UTF8(CJK) supported and tested.


require:
	GCC > 4.1









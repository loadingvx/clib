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

* A Boyer\_moore search function.
	* Boyer\_moore search algorithm was used by GNU grep.
	* Boyer\_moore is even 3~5 times faster than KMP.
	* Writen by C-plusplus.

* Some funtions related to "std::string" or "char\*"
	* split: split a string into vector by a specifid delimeter
	* strip: strip specified chars at both ends.
	* join : python-like join. join strings together with specified delimeter among them.
	* replace: replace a substring into specified new string.

* A bloom filter implementation.
	* this is a powerful algorithm often used by crawler and caching.
	* Tools dealing with big data ,like google bigtable, are also using bloom-filter.
	* A counting mechanism will be added soon to support delete operation.

require:
	gcc/g++ > 4.1









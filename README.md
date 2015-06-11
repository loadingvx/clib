clib
====

This library provides:

* Some funtions related to "std::string" or "char\*"
	* split: split a string into vector by a specifid delimeter
	* strip: strip specified chars at both ends.
	* join : python-like join. join strings together with specified delimeter among them.
	* replace: replace a substring into specified new string.

* A logger
	* Independent, depends on nothing(not like log4cxx).
	* Supporting daily logrotate(not like google log [glog])
	* Supporting multi-thread write
	* Supporting debug/release compiling(erase all debug functions).
	* Unix/Posix/Max OS supported.

* A config parser (cfg)
	* support (string/boolean/int)
	* automatic file-change dectection.
	* support var/env for config like "key=${var}/other/${HOME}"

* A serialized hash map
	* Write/load a serialized hash-map.

* A levenshtein distance(edit distance) calculator.
	* Action costs ( del:insert:replace = 1:1:1 ) was defined to be equal.
	* Change the costs as you wish.
	* UTF8(CJK) supported and tested.

* A Boyer\_moore search function.
	* Boyer\_moore search algorithm was used by GNU grep.
	* Boyer\_moore is even 3~5 times faster than KMP.
	* Writen by C-plusplus.


* A bloom filter implementation.
	* this is a powerful algorithm often used by crawler, cache and bigdata tools(hadoop,google bigtable).
	* A counting mechanism will be added soon to support delete operation.

* An Union\_Find Set(Disjoint Set) algorithm inplementation.
	* Disjoint sets are widly used by computing strongly connected component in graph.
	* Disjoint sets are also used to solve LCA problem, and detect rings in graph.

* An implementation of Skip-list
	* Skip-list is a simple but powful data structure, on average, it's speed is likely RB-tree.
	* Skip-list is used by Redis as the main frame for data storage and retrival.
	* This is a simple implementation, you can use it to store kinds of key-values(being together in a structure)
	* This Skip-list can be used as a hash-map(key value supported), a dynamic sorter, a data set.

* An implementation of Splay\_Tree
	* Splay tree is a self-adjusting tree. it can optimize itself while working.
	* Splay tree can be used as a cache naturally, because of its self-adjust feature. The Elements visited recently are all at the top of the tree.
	* Comparing with RB-tree, splay tree saves more space, because it stores no color or other msg of balance.

* An implementation of base64(decode/encode)
    * Base64 is wildly used by MIME,urlencoding.
    * encoder is done, decoder comming soon.

require:
	gcc/g++ > 4.1









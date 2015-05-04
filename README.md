clib
====

Basic functions for C/C++

#序列化hash

需要一个简单map的序列化功能, 请单独下载static\_hash.h文件使用

#debug工具（log.h/log.c)

一个纯c的log工具， 可以输出行号和文件号， 可以进行检查，
当未定义DEBUG时，debug、check，require, info, notice, warn函数自动在编译时抹去。

* 目前仅支持Linux/Mac OS
* 暂未加入多线程支持
* 可以作为简单log工具和debug工具









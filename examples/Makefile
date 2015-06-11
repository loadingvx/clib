Darwin=1
debug=1
allgxx=1

ifeq ($(Darwin), 1)
	CFLAGS+=-DDarwin
else
	CFLAGS+=-DLinux
endif

ifeq ($(debug), 1)
	CFLAGS+=-g -Wall -DDEBUG
endif

ifeq ($(allgxx), 1)
	CC=g++
else
	CC=gcc
endif


all:testB64 testCfg testHashMap testLog testLevenshtein testBoyer_moore testStr testBloomFilter testDisjoint testSk testSplayTree

testB64:
	$(CC) $(CFLAGS) -O3 b64.usage.c ../log.c ../b64.c -o $@

testHashMap:
	$(CC) $(CFLAGS) -O3 static_hash.usage.cc -o $@

testLog:
	$(CC) $(CFLAGS) log.usage.c ../log.c -o $@ -lpthread -O3

testLevenshtein:
	$(CC) $(CFLAGS) ../log.c ../levenshtein.c levenshtein.usage.c -o $@

testBoyer_moore:
	$(CC) $(CFLAGS) ../log.c ../boyer_moore.cc boyer_moore.usage.cc -o $@

testStr:
	$(CC) $(CFLAGS) ../log.c ../str.cc str.usage.cc -o $@

testBloomFilter:
	$(CC) $(CFLAGS) ../log.c ../str.cc ../bloom_filter.c bloomfilter.usage.cc -o $@

testDisjoint:
	$(CC) $(CFLAGS) union_find.usage.cc ../union_find.c ../log.c -g -o $@

testSk:
	$(CC) $(CFLAGS) skiplist.usage.c ../log.c ../skiplist.c -o $@

testSplayTree:
	$(CC) $(CFLAGS) splaytree.usage.c ../log.c ../splay_tree.c -o $@

testCfg:
	$(CC) $(CFLAGS) cfg.usage.cc ../log.c ../str.cc ../cfg.cc -o $@

clean:
	rm -rf testB64
	rm -rf testLog
	rm -rf testHashMap
	rm -rf testLevenshtein
	rm -rf testBoyer_moore
	rm -rf testBloomFilter
	rm -rf testStr
	rm -rf testDisjoint
	rm -rf testSk
	rm -rf testSplayTree
	rm -rf testCfg
	rm -rf *.log
	rm -rf *.dSYM






all: testHashMap testLog testLevenshtein testBoyer_moore testStr testBloomFilter testDisjoint testSk


testHashMap:
	g++ -Wall -O3 static_hash.usage.cc -o $@

testLog:
	gcc -DDEBUG log.usage.c ../log.c -o $@ -lpthread -O3

testLevenshtein:
	gcc -DDEBUG ../log.c ../levenshtein.c levenshtein.usage.c -o $@

testBoyer_moore:
	g++ -Wall ../log.c ../boyer_moore.cc boyer_moore.usage.cc -DDEBUG -o $@

testStr:
	g++ -Wall ../log.c ../str.cc str.usage.cc -DDEBUG -o $@

testBloomFilter:
	g++ -Wall ../log.c ../str.cc ../bloom_filter.c bloomfilter.usage.cc -DDEBUG -o $@

testDisjoint:
	g++ union_find.usage.cc ../union_find.c ../log.c -DDEBUG -g -o $@

testSk:
	gcc skiplist.usage.c ../log.c ../skiplist.c -DDEBUG -o $@ -g

clean:
	rm -rf testHashMap
	rm -rf testLog
	rm -rf testLevenshtein
	rm -rf testBoyer_moore
	rm -rf testBloomFilter
	rm -rf testStr
	rm -rf testDisjoint
	rm -rf testSk
	rm -rf *.log





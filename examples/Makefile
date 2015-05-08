all: testHashMap testLog testLevenshtein testBoyer_moore


testHashMap:
	g++ -Wall -O3 static_hash.usage.cc -o $@

testLog:
	gcc -DDEBUG log.usage.c ../log.c -o $@ -lpthread -O3

testLevenshtein:
	gcc -DDEBUG ../log.c ../levenshtein.c levenshtein.usage.c -o $@

testBoyer_moore:
	g++ -Wall ../log.c ../boyer_moore.cc boyer_moore.usage.cc -DDEBUG -o $@

clean:
	rm -rf testHashMap
	rm -rf testLog
	rm -rf testLevenshtein
	rm -rf testBoyer_moore
	rm -rf *.log





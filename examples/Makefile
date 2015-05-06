all: testHashMap testLog testLevenshtein


testHashMap:
	g++ -Wall -O3 static_hash.usage.cc -o $@

testLog:
	gcc -DDEBUG log.usage.c ../log.c -o $@ -lpthread -O3

testLevenshtein:
	gcc -DDEBUG ../log.c ../levenshtein.c levenshtein.usage.c -o $@

clean:
	rm -rf testHashMap
	rm -rf testLog
	rm -rf testLevenshtein
	rm -rf *.log





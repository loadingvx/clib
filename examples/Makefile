all: testHashMap testLog


testHashMap:
	g++ -Wall -O3 static_hash.usage.cc -o $@

testLog:
	gcc -DDEBUG log.usage.c ../log.c -o $@ -lpthread -O3

clean:
	rm -rf testHashMap
	rm -rf testLog





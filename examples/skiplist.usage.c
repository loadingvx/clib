//std-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "../skiplist.h"

struct item {
	int   num;
	char name[20];
};

int cmp (void* item, void *key) {
	struct item* it = item;
	int k = *((int*)key);

	if (it->num == k) {
		return 0;
	}

	if (it->num> k) {
		return 1;
	}


	return -1;
}

int itemsize () {
	return sizeof(struct item);
}

void* getKey(void* it) {
	return &(((struct item*)it)->num);
}

int testSkipList() {
	struct sklist* sk = sk_create(cmp, itemsize, getKey);
	check(sk != NULL);

	struct item i = {0xB, "John  White"};
	struct item j = {0xE, "John  Smith"};
	struct item k = {0x8,  "Herry Smith"};

	sk_insert(sk, (void*)(&i));
	sk_insert(sk, (void*)(&j));
	sk_insert(sk, (void*)(&k));

	int key = 8;
	struct item* herry = sk_find(sk, (void*)(&key));
	check( herry != NULL );
	check( strcmp(herry->name, "Herry Smith") == 0 );
	info("found %s\n", herry->name);

	sk_delete(sk, (void*)(&key));

	herry = sk_find(sk, (void*)(&key));
	check(herry == NULL);

	sk_destory(sk);
	return 0;
}



int main(int argc, const char *argv[])
{
	log_init("debug.log", LOG_CONSOLE);

	testSkipList();

	return EXIT_SUCCESS;
}

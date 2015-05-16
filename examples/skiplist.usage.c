//std-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "../skiplist.h"

struct item {
	int  num;
	int  age;
};

int cmp (void* item, void *key) {
	struct item* it = (struct item*)item;
	int k = *((int*)key);

	if (it->num == k) {
		return 0;
	}

	if (it->num < k) {
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

	struct item i = {0xa, 0xAB};
	struct item j = {0xe, 0xAC};
	struct item k = {0x8, 0xAD};
	struct item m = {0xb, 0xAE};
	struct item n = {0x9, 0xAF};

	sk_insert(sk, (void*)(&i)); //a
	sk_insert(sk, (void*)(&k)); //8
	sk_insert(sk, (void*)(&j)); //e
	sk_insert(sk, (void*)(&m)); //b
	sk_insert(sk, (void*)(&n)); //9

	int key = 8;
	const struct item* herry = (const struct item*)sk_find(sk, (void*)(&key));
	check( herry != NULL );
	check( herry->age == 0xAD );
	info("found 0x%X\n", herry->age);

	sk_delete(sk, (void*)(&key));

	herry = (const struct item*)sk_find(sk, (void*)(&key));
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

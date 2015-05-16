//std-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../log.h"
#include "../splay_tree.h"

struct item {
	int  num;
	int  age;
};

int cmp (const void* item, const void *key) {

	const struct item* it = (const struct item*)item;
	const int k = *((const int*)key);

	if (it->num == k) {
		return 0;
	}

	if (it->num > k) {
		return 1;
	}

	return -1;
}

int itemsize () {
	return sizeof(struct item);
}

const void* getKey(const void* it) {
	return &(((struct item*)it)->num);
}

int testSplaytree() {
	struct splay* sptree = sp_create(cmp, itemsize, getKey);

	for (int i = 0; i<30; i++) {
		struct item it = {i, i*10};
		sp_insert(sptree, (void*)(&it));
		info("insert=%d\n", i);
	}
	
	int key= 3;
	info("access = %d\n", key);
	const struct item* n = (const struct item*)sp_access(sptree, (void*)(&key));
	check( n!=NULL );
	check( cmp(n, (void*)(&key)) == 0 );

	info("delate = %d\n", key);
	sp_delete(sptree, (void*)(&key));

	key = 9;
	info("access = %d\n", key);
	n = (const struct item*)sp_access(sptree, (void*)(&key));
	check( n!=NULL );
	check( cmp(n, (void*)(&key)) == 0 );

	info("delate = %d\n", key);
	sp_delete(sptree, (void*)(&key));

	key = 19;
	info("access = %d\n", key);
	n = (const struct item*)sp_access(sptree, (void*)(&key));
	check( n!=NULL );
	check( cmp(n, (void*)(&key)) == 0 );

	info("delate = %d\n", key);
	sp_delete(sptree, (void*)(&key));

	key = 29;
	info("access = %d\n", key);
	n = (const struct item*)sp_access(sptree, (void*)(&key));
	check( n!=NULL );
	check( cmp(n, (void*)(&key)) == 0 );

	info("delate = %d\n", key);
	sp_delete(sptree, (void*)(&key));

	info("destory!\n");
	sp_destory(sptree);

	return 0;
}



int main(int argc, const char *argv[])
{
	log_init("debug.log", LOG_CONSOLE);

	testSplaytree();

	return EXIT_SUCCESS;
}


#include <stdio.h>
#include <stdlib.h>

#include "../static_hash.h"

#include<string>
#include<map>

struct node {
	int  id;
	int  age;
	char name[20];
};

/* gen data */
void dataProducer(const char* filename) {
	std::map<int, struct node> nodes;
	nodes[10] = (struct node){10, 28, "John"};
	nodes[2]  = (struct node){2,  28, "Jane"};
	serialize<int, struct node, std::map<int, struct node> >(nodes, 4, filename);
	printf("Write hash-map into file %s !\n", filename);
}

/* load data */
void dataConsumer(const char* filename) {
	static_hash_map<int, struct node> hash;
	if(hash.load(filename)) {
		printf("Load hash-map from file %s !\n", filename);
	}
	for (int i = 0; i < 10; i++) {
		struct node* pnode = hash[i];
		if (pnode) {
			printf("id:%d age:%d name:%s\n", pnode->id, pnode->age, pnode->name);
		} else {
			printf("key=%d not found\n", i);
		}
	}
}



int main(int argc, const char *argv[])
{
	const char* fname = "user.bin";
	dataProducer(fname);
	dataConsumer(fname);
	return EXIT_SUCCESS;
}



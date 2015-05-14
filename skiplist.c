/*
The MIT License (MIT)

Copyright (c) [2015] [liangchengming]

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "skiplist.h"
#include "log.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef DEBUG
void sk_print_table(struct sklist *self) {
	/* debug */
	for (int i = self->levels; i >= 0; i--) {
		struct sknode* c = self->head;
		while (true) {
			if (c == self->head) {
				printf("l=%d [%ld n=%ld,p=%ld]", i, (size_t)(self->head), (size_t)(c->level)[i].next, (size_t)(self->head->level)[i].pre);
				c = (c->level)[i].next;
				continue;
			}
			if (c == self->tail) {
				printf("[%ld n=%ld,p=%ld]\n", (size_t)self->tail, (size_t)(c->level)[i].next, (size_t)(self->tail->level)[i].pre);
				break;
			}
			printf("<%ld[%X,%X]>,", (size_t)c, ((int*)(c->obj))[0], ((int*)(c->obj))[1]);
			c = (c->level)[i].next;
		}
	}
	printf("********************************************************************************\n");
	/* debug */
}
#else
#define sk_print_tablef(...)
#endif

struct sklist* sk_create(int(*cmp)(void* obj, void *key), int (*obj_size)(), void* (*getKey)(void*)) {
	struct sklist* self = (struct sklist*)malloc(sizeof(struct sklist));
	struct sknode* head = (struct sknode*)malloc(sizeof(struct sknode));
	struct sknode* tail = (struct sknode*)malloc(sizeof(struct sknode));

	srand(time(NULL));

	if (self == NULL || head == NULL || tail == NULL) {
		perror("Skip_list_create");
		notice("Memory Error\n");
		return NULL;
	}

	head->obj  = NULL;
	head->level = (struct sklevel*) malloc(sizeof(struct sklevel));
	(head->level)[0].next = tail;
	(head->level)[0].pre  = NULL;

	tail->obj  = NULL;
	tail->level = (struct sklevel*) malloc(sizeof(struct sklevel));
	(tail->level)[0].pre  = head;
	(tail->level)[0].next = NULL;

	self->levels = 0;

	self->head = head;
	self->tail = tail;

	self->cmp    = cmp;
	self->objsize = obj_size;
	self->getKey = getKey;

	check((self->head->level)[0].next == self->tail);
	check((self->tail->level)[0].pre  == self->head);

	sk_print_table(self);
	return self;
}

int sk_destory(struct sklist* self) {
	if (self == NULL) {
		notice("Free(NULL)\n");
		return -1;
	}

	struct sknode * curr = self->head;
	struct sknode * next = NULL;

	while ( true ) {
		if (curr != self->head && curr != self->tail) {
			free(curr->obj);
		}
		next = (curr->level[0]).next;
		free(curr->level);
		free(curr);
		if (next == NULL) {
			break;
		}
		curr = next;
	}

	free(self);
	return 0;
}

int rand_level() {
	int times = 0;
	while ( rand() & 0x1 ) {
		times++;
	}
	return times;
}

int sk_inner_insert(struct sklist* self, void* obj, struct sknode** path) {
	struct sknode* new_node = (struct sknode*)malloc(sizeof(struct sknode));
	if (new_node == NULL) {
		notice("Memory Error");
		return -1;
	}


	int times = rand_level();
	/* times begin with 0 */
	new_node->level = (struct sklevel*)malloc((times+1) * sizeof(struct sklevel));

	if (times > self->levels) {
		/* new level */
		self->head->level = (struct sklevel*)realloc(self->head->level, sizeof(struct sklevel) * (times+1));
		self->tail->level = (struct sklevel*)realloc(self->tail->level, sizeof(struct sklevel) * (times+1));
	}


	check(path != NULL);

	for (int i = times; i >=0; i--) {

		if (i > self->levels) {
			(self->head->level)[i].pre  = NULL;
			(self->head->level)[i].next = new_node;
			(self->tail->level)[i].pre  = new_node;
			(self->tail->level)[i].next = NULL;
			(new_node->level)[i].pre  = self->head;
			(new_node->level)[i].next = self->tail;
			continue;
		}

		(self->head->level)[i].pre  = NULL;
		(self->tail->level)[i].next = NULL;

		(new_node->level)[i].pre = path[i];
		(new_node->level)[i].next = ((path[i])->level)[i].next;

		((((path[i])->level)[i].next)->level)[i].pre = new_node;
		((path[i])->level)[i].next = new_node;
	}

	if (times> self->levels) {
		self->levels = times;
	}

	int obj_size = (*(self->objsize))();
	new_node->obj = malloc(obj_size);
	memcpy(new_node->obj, obj, obj_size); /* payload */


	return 0;
}


int sk_insert(struct sklist* self, void* obj) {

	if (self == NULL) {
		notice("Searching an empty skip_list\n");
		return -1;
	}

	struct sknode** path = (struct sknode**)malloc( (self->levels+1) * sizeof(struct sknode*));
	memset(path, 0, sizeof(struct sknode*)* (self->levels+1));

	struct sknode* curr = self->head;
	struct sknode* next = NULL;
	int l = self->levels;

	while ( true ) {

		path[l] = curr;
		next = ((curr->level)[l]).next;
		info("%d: next=%X->%X\n", l, curr, next);

		if (next == self->tail) {
			if (l - 1 >= 0) {
				l--;
				continue;
			}
			sk_inner_insert(self,obj, path);
			free(path);
			sk_print_table(self);
			return 0;
		}

		int ret = (*(self->cmp))(next->obj, (*(self->getKey))(obj));

		if (ret == 0) {
			info("duplicate Key, Insertion Canceled.\n");
			return -1;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}

		l--;
		if (l < 0) {
			sk_inner_insert(self, obj, path);
			free(path);
			sk_print_table(self);
			return 0;
		}
	}

	return 0;
}


int sk_inner_delete(struct sklist* self, int l, struct sknode *curr) {

	for (int i = l; i >= 0; i--) {
		struct sknode* previous = (curr->level)[i].pre;
		(previous->level)[i].next = (curr->level)[i].next;
		(((curr->level)[i].next)->level)[i].pre = previous;
	}

	free(curr->obj);
	free(curr->level);
	free(curr);

	int ll = self->levels;
	while ( (self->head->level)[ll].next == self->tail) {
		ll--;
	}
	if (ll != self->levels) {
		self->tail->level = (struct sklevel*)realloc(self->tail->level, (ll+1) * sizeof(struct sklevel) );
		self->head->level = (struct sklevel*)realloc(self->head->level, (ll+1) * sizeof(struct sklevel) );
		self->levels = ll;
	}

	return 0;
}

int sk_delete(struct sklist* self, void* key) {

	if (self == NULL) {
		notice("Searching an empty skip_list\n");
		return -1;
	}

	struct sknode* curr = self->head;
	struct sknode* next = NULL;
	int l = self->levels;

	while ( true ) {

		next = ((curr->level)[l]).next;

		if (curr == self->head) {
			curr = next;
			continue;
		}

		if (curr == self->tail) {
			curr = (self->tail->level)[l].pre;
			l--;
			if (l < 0) {
				return -1; /* end */
			}
			continue;
		}

		check(curr->obj != NULL);
		int ret = (*(self->cmp))(curr->obj, key);

		if (ret == 0) {
			sk_inner_delete(self, l, curr);
			sk_print_table(self);
			return 0;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}

		curr = (curr->level)[l].pre;
		l--;
		if (l < 0) {
			return -1;
		}
	}

	return 0; /* should not be here */

}

void* sk_find(struct sklist* self, void* key) {

	if (self == NULL) {
		notice("Searching an empty skip_list\n");
		return NULL;
	}

	struct sknode* curr = self->head;
	struct sknode* next = NULL;
	int l = self->levels;

	while ( true ) {

		next = ((curr->level)[l]).next;
		if (next == self->tail) {
			l--;
			if (l < 0) {
				return NULL; /* end */
			}
			continue;
		}

		int ret = (*(self->cmp))(next->obj, key);

		if (ret == 0) {
			return next->obj;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}

		l--;
		if (l < 0) {
			return NULL;
		}
	}

	return NULL; /* should not be here */
}


#ifdef __cplusplus
}
#endif



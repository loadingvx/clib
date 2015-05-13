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
		struct sknode* n = NULL;
		while (true) {
			if (c == NULL) {
				info("abort\n");
			}
			n = (c->level)[i].next;
			if (c == self->head) {
				printf("l=%d [head:%x next = %x, pre=%x]", i, (unsigned int)self->head, (unsigned int)n, (unsigned int)((self->head->level)[i].pre));
				c = n;
				continue;
			}
			if (c == self->tail) {
				printf("[tail:%x next = %x, pre=%x]\n", (unsigned int)self->tail, (unsigned int)n, (unsigned int)((self->tail->level)[i].pre));
				break;
			}
			printf("<%x,%x>,", (unsigned int)(c), ((int*)(c->obj))[0]);
			c = n;
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

	self->head   = head;
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

int sk_inner_insert(struct sklist* self, void* obj, struct sknode* curr) {
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
		self->head->level = (struct sklevel*)realloc(self->head->level, sizeof(struct sklevel) * times+1);
		self->tail->level = (struct sklevel*)realloc(self->tail->level, sizeof(struct sklevel) * times+1);
	}


	check(curr != NULL);

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

		(new_node->level)[i].pre = curr;
		(new_node->level)[i].next = (curr->level)[i].next;

		(((curr->level)[i].next)->level)[i].pre = new_node;
		(curr->level)[i].next = new_node;
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

	struct sknode* curr = self->head;
	struct sknode* next = NULL;
	int l = self->levels;

	while ( true ) {
		check(curr != NULL);
		next = ((curr->level)[l]).next;

		if (curr == self->head) {
			curr = next;
			continue;
		}

		if (curr == self->tail) {
			if (l - 1 >= 0) {
				curr = (curr->level)[l].pre;
				l--;
				continue;
			}
			sk_inner_insert(self,obj, (curr->level)[l].pre);
			sk_print_table(self);
			return 0;
		}

		int ret = (*(self->cmp))(curr->obj, (*(self->getKey))(obj));

		if (ret == 0) {
			return -1;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}
		
		if (l == 0) {
			sk_inner_insert(self, obj, (curr->level)[l].pre);
			sk_print_table(self);
			return 0;
		}

		curr = (curr->level)[l].pre;
		l--;

	}

	return 0;
}


int sk_inner_delete(struct sklist* self, int l, struct sknode *curr) {

	for (int i = l; i >= 0; i--) {
		struct sknode* tmp = (curr->level)[i].pre;
		(tmp->level)[i].next = (curr->level)[i].next;
		(((tmp->level)[i].next)->level)[i].pre = tmp;
	}

	free(curr->obj);
	free(curr->level);
	free(curr);

	while ( (self->head->level)[self->levels].next == self->tail) {
		self->tail->level = realloc(self->tail->level, (self->levels - 1) * sizeof(struct sklevel) );
		self->head->level = realloc(self->head->level, (self->levels - 1) * sizeof(struct sklevel) );
		self->levels -= 1;
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
			return -1;
		}

		int ret = (*(self->cmp))(curr->obj, key);

		if (ret == 0) {
			sk_inner_delete(self, l, curr);
			return 0;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}
		
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

		if (curr == self->head) {
			curr = next;
			continue;
		}

		if (curr == self->tail) {
			curr = (self->tail->level)[l].pre;
			l--;
			if (l < 0) {
				return NULL; /* end */
			}
			continue;
		}

		check(curr->obj != NULL);
		int ret = (*(self->cmp))(curr->obj, key);

		if (ret == 0) {
			return curr->obj;
		}

		if (ret < 0) {
			curr = next;
			continue;
		}

		l--;
		if (l < 0) {
			return NULL;
		}
		curr = (curr->level)[l].pre;
	}

	return NULL; /* should not be here */
}


#ifdef __cplusplus
}
#endif



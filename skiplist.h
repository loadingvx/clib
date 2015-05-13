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

#ifndef _H
#define _H

#ifdef __cplusplus
extern "C" {
#endif

struct sklevel {
	struct sknode* next;
	struct sknode* pre;
};

struct sknode {
	void* obj;
	struct sklevel *level;
};

struct sklist {
	struct sknode* head;
	struct sknode* tail;
	int levels;
	int(*cmp)(void* obj, void *key);
	int (*objsize)();
	void* (*getKey)(void*);
};

struct sklist* sk_create(int(*cmp)(void* obj, void *key), int (*objsize)(), void* (*getKey)(void*));
int sk_insert(struct sklist* self, void* obj);
int sk_delete(struct sklist* self, void* key);
void* sk_find(struct sklist* self, void* key);
int sk_destory(struct sklist* self);

#ifdef __cplusplus
}
#endif

#endif


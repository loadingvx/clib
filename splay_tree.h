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

#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

struct splay_node {
	void * value;
	struct splay_node* f;
	struct splay_node* l;
	struct splay_node* r;
};

struct splay {
	int count;
	struct splay_node *root;
	int (*cmp)(const void*, const void*);
	int (*objsize)();
	const void* (*getKey)(const void*);
};

struct splay* sp_create(int (*cmp)(const void*, const void*), int (*objsize)(), const void* (*getKey)(const void*) );
const void* sp_access(struct splay* self, const void* key);
const void* sp_find(struct splay* self, const void* key);
int sp_delete(struct splay* self, const void* key);
int sp_insert(struct splay* self, const void* obj);
int sp_destory(struct splay* self);


#ifdef __cplusplus
}
#endif

#endif


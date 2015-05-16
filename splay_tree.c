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

#include "splay_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef DEBUG
#define splay_print(...)
/*
void printTree (struct splay_node*root, int blk) {
	for(int i=0;i<blk;i++) {
		printf("  ");
	}
	if (root) {
		printf("├──<%d>\n", *(int*)(root->value));
	} else {
		printf("├──</>\n");
		return;
	}
	printTree(root->l,blk+1);
	printTree(root->r,blk+1);
}
void splay_print(struct splay* self) {
	struct splay_node* c = self->root;
	printTree(c, 0);
}
*/
#else
#define splay_print(...)
#endif

enum filiation { none=1, l_son, r_son };

struct splay* sp_create(int (*cmp)(const void*, const void*), int (*objsize)(), const void* (*getKey)(const void*) ){

	struct splay* self = (struct splay*) malloc (sizeof(struct splay) * 1);

	if (self == NULL) {
		perror("splay-tree-create memory error");
		return NULL;
	}

	self->count   = 0;
	self->root    = NULL;
	self->cmp     = cmp;
	self->objsize = objsize;
	self->getKey  = getKey;

	return self;
}

bool is_root(struct splay* self, struct splay_node* n) {
	return n == self->root;
}

enum filiation filiation_type(struct splay_node* n1, struct splay_node* n2) {

	if (n1 == NULL) {
		return none;
	}

	if (n1->l == n2) {
		return l_son;
	}
	if (n1->r == n2) {
		return r_son;
	}

	return none;
}


void zg(struct splay_node* g, struct splay_node* p, struct splay_node* x, enum filiation filia) {

	check( filia == l_son || filia == r_son );

	if (filia == l_son) {          /* zig */
		if (x->r != NULL) {
			x->r->f = p;
		}
		p->l = x->r;
		x->r = p;
	} else if (filia == r_son) {   /* zag */
		if (x->l != NULL) {
			x->l->f = p;
		}
		p->r = x->l;
		x->l = p;
	}

	p->f = x;
	x->f = g;
}

/* zig-zig && zag-zag */
void zzg(struct splay_node*g, struct splay_node*p, struct splay_node*x, enum filiation filia0, enum filiation filia1)
{
	zg(g->f, g, p, filia1);
	if (p->f != NULL) {
		if (filia0 == l_son) {
			p->f->l = p;
		} else {
			p->f->r = p;
		}
	}
	zg(p->f, p, x, filia1);
	if (x->f != NULL) {
		if (filia0 == l_son) {
			x->f->l = x;
		} else {
			x->f->r = x;
		}
	}
}

/* zig-zag && zag-zig */
void zg_zg(struct splay_node*g, struct splay_node*p, struct splay_node*x, enum filiation filia0, enum filiation filia1, enum filiation filia2) {
	check(filia1 != none);
	check(filia2 != none);
	check(filia1 != filia2);

	zg(g, p, x, filia1);
	if (filia0 == l_son) {
		g->l = x;
	} else {
		g->r = x;
	}
	zg(g->f, g, x, filia2);
	if (filia0 == l_son) {
		g->f->l = x;
	} else {
		g->f->r = x;
	}
}


void self_optimize(struct splay* self, struct splay_node* x) {

	/* this is so called "splay" */

	/* Case 1 (zig).
	 * If p(x),the parent of x, is the tree root, rotate the edge joining x
	 * with p(x). (Thiscase is terminal.)
	 *
	 * Case 2 (zig-zig).
	 * If p(x) is not the root and x and p(x) are both left or both right
	 * children, rotate the edge joining p(x) with its grandparent
	 * g(x) and then rotate the edge joining x with p (x) .
	 *
	 * Case3(zig-zag).
	 * Ifp(x)isnottherootandxisaleftchildandp(x)arightchild,orvice- versa,
	 * rotate the edge joining x with p(x) and then rotate the edge joining x
	 * with the new p(x).
	 *
	 *                         --D. D. SLEATOR AND R. E. TARJAN
	 */
	while (! is_root(self, x) ) {

		struct splay_node *p = x->f;
		struct splay_node *g = p->f;

		if ( is_root(self, p) ) {
			zg(g, p, x, filiation_type(p, x));
			self->root = x;
		} else {
			struct splay_node *o = g->f;

			enum filiation f0 = filiation_type(o, g);
			enum filiation f1 = filiation_type(g, p);
			enum filiation f2 = filiation_type(p, x);

			if ( f1 == f2 ) {
				zzg(g, p , x, f0, f1);
			} else {
				zg_zg(g, p, x, f0, f1, f2);
			}

			if (f0 == none) {
				self->root = x;
			}
		}

		splay_print(self);
	}

}


const void* sp_find(struct splay* self, const void* key) {
	struct splay_node* c = self->root;

	while ( true ) {

		int ret = (*(self->cmp))(c->value, key);
		if ( ret == 0 ) {
			self_optimize(self, c);
			return c->value;
		}

		if ( ret > 0 ) {
			if( c->l != NULL ) {
				c = c->l;
				continue;
			}
			self_optimize(self, c);
			return NULL;
		}

		if ( ret < 0 ) {
			if ( c->r != NULL ) {
				c = c->r;
				continue;
			}
			self_optimize(self, c);
			return NULL;
		}

	}
	return NULL;
}

const void* sp_access(struct splay* self, const void* key) {
	return sp_find(self, key);
}

struct splay_node* sp_inner_insert(struct splay* self, struct splay_node* c,  const void* obj, enum filiation filia) {

	struct splay_node* x = (struct splay_node*) malloc(sizeof(struct splay_node));

	int sz = (*(self->objsize))();
	x->value = malloc(sz);
	memcpy(x->value, obj, sz);

	if (self->root == NULL) {
		self->root = x;
	} else {
		if ( filia == l_son) {
			c->l = x;
		} else {
			c->r = x;
		}
	}

	x->f = c;
	x->l = NULL;
	x->r = NULL;

	self->count += 1;
	splay_print(self);
	return x;
}

int sp_insert(struct splay* self, const void* obj) {
	struct splay_node* c = self->root;

	while ( true ) {
		if (c == NULL) {
			sp_inner_insert(self, c, obj, none);
			return 0;
		}

		int ret = (*(self->cmp))(c->value, (*(self->getKey))(obj));
		if ( ret == 0 ) {
			/* duplication of key */
			self_optimize(self, c);
			return -1;
		}

		if ( ret > 0 ) {
			if( c->l != NULL ) {
				c = c->l;
				continue;
			}
			sp_inner_insert(self,c,obj,l_son);
			return 0;
		}

		if ( ret < 0 ) {
			if ( c->r != NULL ) {
				c = c->r;
				continue;
			}
			sp_inner_insert(self,c,obj,r_son);
			return 0;
		}

	}
	return 0;
}

struct splay_node* sp_inner_delete(struct splay* self, struct splay_node* c) {
	int f = filiation_type(c->f, c);
	struct splay_node* replace_node = NULL;

	if (c->r != NULL) { /* replace = min of right */
		replace_node = c->r;
		while (replace_node->l != NULL) {
			replace_node = replace_node->l;
		}
		if (replace_node != c->r) {

			replace_node->f->l = replace_node->r;
			if ( replace_node->r != NULL ) {
				replace_node->r->f = replace_node->f;
			}

			replace_node->f = c->f;
			replace_node->r = c->r;
			replace_node->l = c->l;

			if (replace_node->l != NULL) {
				replace_node->l->f = replace_node;
			}

		} else {
			replace_node->f = c->f;
			replace_node->l = c->l;
		}
	} else if (c->l != NULL) {
		replace_node = c->l;
		while ( replace_node->r != NULL) {
			replace_node = replace_node->r;
		}
		if (replace_node != c->l){
			replace_node->f->r = replace_node->l;
			if(replace_node->l != NULL) {
				replace_node->l->f = replace_node->f;
			}

			replace_node->f = c->f;
			replace_node->r = c->r;
			replace_node->l = c->l;

			if (replace_node->r != NULL) {
				replace_node->r->f = replace_node;
			}

		} else {
			replace_node->f = c->f;
			replace_node->r = c->r;
		}
	}

	if ( f == none ) {
		self->root = replace_node;
	} else if (f == l_son) {
		c->f->l = replace_node;
	} else if (f == r_son) {
		c->f->r = replace_node;
	}

	free(c->value);
	free(c);
	splay_print(self);
	return replace_node;
}

int sp_delete(struct splay* self, const void* key) {
	struct splay_node* c = self->root;

	while ( true ) {
		if (c == NULL) {
			return 0;
		}

		int ret = (*(self->cmp))(c->value, (*(self->getKey))(key));
		if ( ret == 0 ) {
			sp_inner_delete(self, c);
			return 0;
		}

		if ( ret > 0 ) {
			if( c->l != NULL ) {
				c = c->l;
				continue;
			}
			return -1;
		}

		if ( ret < 0 ) {
			if ( c->r != NULL ) {
				c = c->r;
				continue;
			}
			return -1;
		}
	}
	return -1;
}

void sp_inner_destory(struct splay_node* c) {
	if (c->l != NULL) {
		sp_inner_destory(c->l);
	}
	if (c->r != NULL) {
		sp_inner_destory(c->r);
	}
	free(c);
}

int sp_destory(struct splay* self) {
	struct splay_node* c = self->root;
	sp_inner_destory(c);
	free(self);
	return 0;
}


#ifdef __cplusplus
}
#endif


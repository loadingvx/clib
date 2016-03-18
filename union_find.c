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

#include "union_find.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif


struct UFS* ufs_open(int _n) {
	if (_n <= 0) {
		notice("Empty union_find set.\n");
		return NULL;
	}

	int* sym = (int*) malloc( sizeof(int) * _n );
	if (sym == NULL) {
		perror("ufs_open");
		notice("Memory Error");
		return NULL;
	}

	int* weights = (int*) malloc( sizeof(int) * _n );
	if (weights == NULL) {
		perror("ufs_open");
		notice("Memory Error");
		return NULL;
	}

	struct UFS* uf = (struct UFS*) malloc(sizeof(struct UFS));
	if (uf == NULL) {
		perror("ufs_open");
		notice("Memory Error");
		free(sym);
		return NULL;
	}

	uf->count   = _n;
	uf->sym     = sym;
	uf->weights = weights;

	for (int  i = 0; i < _n; i++) {
		sym[i] = i;
		weights[i] = 1;
	}

	return uf;
}




int  ufs_find(struct UFS* uf, int p) {
	if (uf == NULL) {
		notice("searching an EMPTY union find set!\n");
		return -1;
	}

	while ((uf->sym)[p] != p) {
		p = (uf->sym)[p];
		/* change (uf->sym)[p] to his grandparent, speed up find function */
		(uf->sym)[p] = (uf->sym)[(uf->sym)[p]];
	}

	return p;
}

int  ufs_union(struct UFS* uf, int p, int q) {
	if (uf == NULL) {
		notice("Operation EMPTY union find set.\n");
		return -1;
	}

	int symp = ufs_find(uf, p);
	int symq = ufs_find(uf, q);

	if (symp == symq) {
		return 0;
	}

	/* _lock */
	/* weight means the number of current set, by balancing weights, to speed up
	 * find function
	 */
	if ((uf->weights)[symp] < (uf->weights)[symq]) {
		(uf->sym)[symp] = symq;
		(uf->weights)[symq] = ((uf->weights)[symp] +1 > (uf->weights)[symq] ? (uf->weights)[symp] +1: (uf->weights)[symq]);
	} else {
		(uf->sym)[symq] = symp;
		(uf->weights)[symp] = ((uf->weights)[symq] +1 > (uf->weights)[symp] ? (uf->weights)[symq] +1: (uf->weights)[symp]);
	}
	/* unlock */

	__sync_fetch_and_sub(&(uf->count), 1);
	return 0;
}

bool ufs_connected(struct UFS* uf, int p, int q) {
	return ufs_find(uf, p) == ufs_find(uf, q);
}

int ufs_close(struct UFS* uf) {
	if (uf == NULL) {
		notice("free(NULL)\n");
		return -1;
	}

	free(uf->sym);
	uf->sym= NULL;

	free(uf->weights);
	uf->weights = NULL;

	return 0;
}


int  ufs_count(struct UFS* uf) {
	if (uf != NULL) {
		return uf->count;
	}
	notice("Empty union_find set.\n");
	return -1;
}



#ifdef __cplusplus
}
#endif

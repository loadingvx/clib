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


#include "bloom_filter.h"
#include <stdint.h>                             /* typedef int int32_t; */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "log.h"


#ifdef __cplusplus
extern "C" {
#endif


int32_t RSHash(const char* str)     /*Hash by Robert Sedgwicks */
{
	int32_t b    = 378551;
	int32_t a    = 63689;
	int32_t hash = 0;
	for(int32_t i = 0; i < strlen(str); i++) {
		hash = hash * a + *(str+i);
		a = a * b;
	}
	return hash;
}


int32_t JSHash(const char *str)     /*Hash by Justin Sobel      */
{
	int32_t hash = 1315423911;
	for(int32_t i = 0; i < strlen(str); i++) {
		hash ^= ((hash << 5) + str[i] + (hash >> 2));
	}
	return hash;
}

int32_t PJWHash(const char* str)     /*Hash by PJWHash           */
{
	int32_t BitsInUnsignedInt = (int32_t)(4 * 8);
	int32_t ThreeQuarters     = (int32_t)((BitsInUnsignedInt  * 3) / 4);
	int32_t OneEighth         = (int32_t)(BitsInUnsignedInt / 8);
	int32_t HighBits          = (int32_t)(0xFFFFFFFF) << (BitsInUnsignedInt - OneEighth);
	int32_t hash              = 0;
	int32_t test              = 0;
	for(int32_t i = 0; i < strlen(str); i++) {
		hash = (hash << OneEighth) + str[i];
		if((test = hash & HighBits)  != 0) {
			hash = (( hash ^ (test >> ThreeQuarters)) & (~HighBits));
		}
	}
	return hash;
}

int32_t ELFHash(const char* str)       /* ELFHash                      */
{
	int32_t hash = 0;
	int32_t x    = 0;
	for(int i = 0; i < strlen(str); i++) {
		hash = (hash << 4) + str[i];
		if((x = hash & 0xF0000000L) != 0) {
			hash ^= (x >> 24);
		}
		hash &= ~x;
	}
	return hash;
}

int32_t BKDRHash(const char* str)  /* Brian Kernighan & Dennis Ritchie */
{
	int32_t seed = 131; // 31 131 1313 13131 131313 etc..
	int32_t hash = 0;
	for(int i = 0; i < strlen(str); i++) {
		hash = (hash * seed) + str[i];
	}
	return hash;
}


int32_t SDBMHash(const char* str)  /* From SDBM library  */
{
	int32_t hash = 0;
	for(int i = 0; i < strlen(str); i++) {
		hash = str[i] + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

int32_t DJBHash(const char* str)  /* Daniel J.Bernstein */
{
	int32_t hash = 5381;
	for(int i = 0; i < strlen(str); i++) {
		hash = ((hash << 5) + hash) + str[i];
	}
	return hash;
}


int32_t DEKHash(const char* str)  /* Knuth */
{
	int len = strlen(str);
	int32_t hash = len;
	for(int i = 0; i < len; i++) {
		hash = ((hash << 5) ^ (hash >> 27)) ^ str[i];
	}
	return hash;
}

int32_t APHash(const char* str)  /* Arash Partow    */
{
	int32_t hash = 0xAAAAAAAA;
	for(int i = 0; i < strlen(str); i++) {
		if ((i & 1) == 0) {
			hash ^= ((hash << 7) ^ str[i] * (hash >> 3));
		} else {
			hash ^= (~((hash << 11) + str[i] ^ (hash >> 5)));
		}
	}
	return hash;
}


int32_t (*hashfuncs[])(const char*) = {
	RSHash,  JSHash,   PJWHash,
	ELFHash, BKDRHash, SDBMHash,
	DJBHash, DEKHash,  APHash,
	NULL
};
#define HASH_NUM 9

char mask[] = {
	0x01,
	0x02,
	0x04,
	0x08,
	0x10,
	0x20,
	0x40,
	0x80
};


struct bloom_filter* bopen(int _n, float p) {
	struct bloom_filter* bfilter = (struct bloom_filter*)malloc(sizeof(struct bloom_filter));
	if (bfilter == NULL) {
		perror("Fatal in bopen<bloom filter>:");
		return NULL;
	}

	if (p > 1.0) {
		fprintf(stderr, "probability must in range [0, 1.0]\n");
		return NULL;
	}

	bfilter->n = 0;
	bfilter->m = ceil(-1 * _n * log(p) / pow(float(log(2)), 2));
	int k = ceil( bfilter->m / _n * log(2) );
	bfilter->k = ( k > HASH_NUM ? HASH_NUM : k );
	bfilter->bits = (char*)malloc(bfilter->m);
	memset(bfilter->bits, 0x0, bfilter->m);
	if (bfilter->bits == NULL) {
		perror("Fatal in bopen<bloom filter>:");
		return NULL;
	}

	info("Create bloomfilter ok! params <n:%d, m:%d, k:%d>\n", _n, bfilter->m, bfilter->k);
	return bfilter;
}

int bclose(struct bloom_filter* bfilter) {

	if(bfilter == NULL) {
		notice("tring to free(NULL) !\n");
		return -1;
	}

	free(bfilter->bits);
	bfilter->bits = NULL;

	free(bfilter);
	bfilter = NULL;

	return 0;
}


int bset(struct bloom_filter *bfilter, const char* key) {

	int _k = 0;
	int32_t (*func) (const char*) = NULL;

	while ( true ) {
		func = hashfuncs[_k];
		if(func == NULL || _k == bfilter->k) {
			break;
		}

		int32_t offset = abs((*func)(key)) % (bfilter->m * 8);
		int i = offset / 8;
		int j = offset % 8;

		info("offset = %d, i = %d, j = %d\n", offset, i, j);

		(bfilter->bits)[i] = ((bfilter->bits)[i]) | mask[j];

		_k++;
	}

	__sync_fetch_and_add(&(bfilter->n), 1);

	return 0;
}



bool bfind(struct bloom_filter *bfilter, const char* key) {

	int _k = 0;
	int32_t (*func) (const char*) = NULL;

	while ( true ) {
		func = hashfuncs[_k];
		if(func == NULL || _k == bfilter->k) {
			break;
		}

		int32_t offset = abs((*func)(key)) % (bfilter->m * 8);
		int i = offset / 8;
		int j = offset % 8;

		if( ! (((bfilter->bits)[i]) & mask[j]) ) {
			return false;
		}
		_k++;
	}

	return true;
}

int bdump_bits(struct bloom_filter* bfilter) {
	FILE *f = fopen("bloom.dump", "w+");
	fwrite(bfilter->bits, bfilter->m, 1, f);
	fclose(f);
	return 0;
}

float breliability(struct bloom_filter* bfilter) {
	info("current n=%d, m=%d, k=%d\n", bfilter->n, bfilter->m*8, bfilter->k);
	/* bdump_bits(bfilter); */
	return 1 - pow(float(1-exp(-(bfilter->k)*(bfilter->n + 0.5)/(bfilter->m -1 ))), bfilter->k );
}


#ifdef __cplusplus
}
#endif


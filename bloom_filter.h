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

#ifndef BLOOM_FILTER_H
#define BLOOM_FILTER_H

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

struct bloom_filter {
	char* bits;
	int   m;
	int   k;
	int   n;
};

/*
 * \name bloom_filter creater.
 * \param
 *	int _n : number of keys
 */
struct bloom_filter* bopen(int _n);

int bclose(struct bloom_filter* bfilter);

int bset(struct bloom_filter *bfilter, const char* key);

bool bfind(struct bloom_filter *bfilter, const char* key);

float breliability(struct bloom_filter* bfilter);




#ifdef __cplusplus
}
#endif


#endif


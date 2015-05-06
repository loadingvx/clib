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

/*
 This is a implemention of levenshtein distance(edit distance).

 Define your own cost for actions(replace,insert and delete) and make sure
 the three values fulfill the inequality of triangle.

 { replace < delete + insert }

*/

#include "levenshtein.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

int tri_min(int x, int y, int z) {
	if (x < y) {
		return (x < z? x:z);
	}
	return (y < z? y:z);
}


int levenshtein(const char* str0, int len0, const char* str1, int len1) {

	check(str0 != NULL && len0 > 0);
	check(str1 != NULL && len1 > 0);

	int n = len0 + 1; /* row */
	int m = len1 + 1; /* col */

	int *buf = (int *)malloc( n*m );

	for (int i = 0; i < n; i++) {
		buf[i*m] = i;
	}

	for (int j = 0; j < m; j++) {
		buf[j] = j;
	}

	for (int i = 1; i < n; i++) {
		for (int j = 1; j < m; j++) {
			if (str0[i-1] == str1[j-1]) {
				buf[i*m+j] = buf[(i-1)*m+(j-1)];
			} else {
				buf[i*m+j] = tri_min(buf[(i-1)*m+(j-1)] + REPLACE_COST,
							buf[i*m+(j-1)] + INSERT_COST,
							buf[(i-1)*m+j] + DELETE_COST);
			}
		}
	}

	int distance = buf[ n*m -1 ];

	free(buf);
	return distance;

}

#ifdef __cplusplus
}
#endif




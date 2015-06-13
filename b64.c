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

#include "b64.h"
#include "log.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

static const char base[] = {
	'A','B','C','D','E','F','G','H',
	'I','J','K','L','M','N','O','P',
	'Q','R','S','T','U','V','W','X',
	'Y','Z','a','b','c','d','e','f',
	'g','h','i','j','k','l','m','n',
	'o','p','q','r','s','t','u','v',
	'w','x','y','z','0','1','2','3',
	'4','5','6','7','8','9','+','/'
};

static const char rebase[] = {
	0,  0,  0,  0,  0,  0,  0,  0,  /*   padding  */
	0,  0,  0,  0,  0,  0,  0,  0,  /*   padding  */
	0,  0,  0,  0,  0,  0,  0,  0,  /*   padding  */
	0,  0,  0,  0,  0,  0,  0,  0,  /*   padding  */
	0,  0,  0,  0,  0,  0,  0,  0,  /*   padding  */
	0,  0,  0,  62, 0,  0,  0,  63, /*   '+'  '/' */
	52, 53, 54, 55, 56, 57, 58, 59, /*    0-9     */
	60, 61, 0,  0,  0,  0,  0,  0,  /*    0-9     */
	0,  0,  1,  2,  3,  4,  5,  6,  /*    A-Z     */
	7,  8,  9,  10, 11, 12, 13, 14, /*    A-Z     */
	15, 16, 17, 18, 19, 20, 21, 22, /*    A-Z     */
	23, 24, 25, 0,  0,  0,  0,  0,  /*    A-Z     */
	0,  26, 27, 28, 29, 30, 31, 32, /*    a-z     */
	33, 34, 35, 36, 37, 38, 39, 40, /*    a-z     */
	41, 42, 43, 44, 45, 46, 47, 48, /*    a-z     */
	49, 50, 51                      /*    a-z     */
};

void charXto4(char* buf, const char* xbytes, int x) {
	check(buf != NULL && xbytes != NULL);

	*(buf+0) = base[xbytes[0]>>2];
	if (x == 1) {
		*(buf+1) = base[(xbytes[0]&0x3)<<4];
		*(buf+2) = '=';
		*(buf+3) = '=';
		return;
	}

	if (x == 2) {
		*(buf+1) = base[(xbytes[0]&0x3)<<4 | (xbytes[1]>>4)];
		*(buf+2) = base[(xbytes[1]&0xF)<<2];
		*(buf+3) = '=';
		return;
	}

	/* x == 3 */
	*(buf+1) = base[(xbytes[0]&0x3)<<4 | (xbytes[1]>>4)];
	*(buf+2) = base[(xbytes[1]&0xF)<<2 | (xbytes[2]>>6)];
	*(buf+3) = base[xbytes[2]&0x3F];
	return;
}


char* b64encode(const char* raw) {
	check(raw != NULL);
	int raw_len = strlen(raw);
	int encode_len = raw_len % 3 == 0 ? raw_len / 3 * 4: (raw_len / 3 + 1) * 4;
	char* buffer = (char*) malloc(sizeof(char) * encode_len + 1);
	memset(buffer, 0x0, encode_len + 1);
	int idx = 0;

	char ch3[3];
	for (int i = 0; i < raw_len; ) {
		memset(ch3, 0x0, 3);
		if (raw_len - i >= 3) {
			strncpy(ch3, raw + i, 3);
			charXto4(buffer+idx, ch3, 3);
			idx+=4;
			i+=3;
			continue;
		}
		strncpy(ch3, raw + i, raw_len - i);
		charXto4(buffer+idx, ch3, raw_len - i);
		idx+=4;
		i+=raw_len - i;
	}

	buffer[sizeof(char) * encode_len + 1] = 0;
	return buffer;
}

void char4toX(char* buf, const char* ch4) {
	int x = 4;
	if (ch4[3] == '=') {
		x--;
	}
	if (ch4[2] == '=') {
		x--;
	}

	if (x == 2) {
		*(buf + 0) = (rebase[int(ch4[0])]&0x3F)<<2 | (rebase[int(ch4[1])]&0x30)>>4; /* 6 + 2 */
		return;
	}

	if (x == 3) {
		*(buf + 0) = (rebase[int(ch4[0])]&0x3F)<<2 | (rebase[int(ch4[1])]&0x30)>>4; /* 6 + 2 */
		*(buf + 1) = (rebase[int(ch4[1])]&0x0F)<<4 | (rebase[int(ch4[2])]&0x3C)>>2; /* 4 + 4 */
		return;
	}

	*(buf + 0) = (rebase[int(ch4[0])]&0x3F)<<2 | (rebase[int(ch4[1])]&0x30)>>4; /* 6 + 2 */
	*(buf + 1) = (rebase[int(ch4[1])]&0x0F)<<4 | (rebase[int(ch4[2])]&0x3C)>>2; /* 4 + 4 */
	*(buf + 2) = (rebase[int(ch4[2])]&0x03)<<6 | (rebase[int(ch4[3])]&0x3F)   ; /* 2 + 6 */

}

char* b64decode(const char* raw) {
	check(raw != NULL);

	int raw_len = strlen(raw);
	check(raw_len % 4 == 0);

	char* buffer = (char*) malloc(raw_len / 4 * 3 + 1);
	memset(buffer, 0x0, raw_len / 4 * 3 + 1);

	char ch4[4];
	for (int idx=0,raw_idx=0; raw_idx < raw_len; idx+=3, raw_idx+=4) {
		memset(ch4, 0x0, 4);
		strncpy(ch4, raw+raw_idx, 4);
		char4toX(buffer+idx, ch4);
	}

	return buffer;
}

#ifdef __cplusplus
}
#endif



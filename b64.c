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

char* b64decode(const char* raw) {
	check(raw != NULL);
	return NULL;
}

#ifdef __cplusplus
}
#endif



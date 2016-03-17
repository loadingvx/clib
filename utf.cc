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

#include "utf.h"
#include "string.h"

int utf8_offset(const char* ptr) {
	/* UTF-8 编码规则
	 * 1字节 0xxxxxxx
	 * 2字节 110xxxxx 10xxxxxx
	 * 3字节 1110xxxx 10xxxxxx 10xxxxxx
	 * 4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	 * 5字节 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	 * 6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	 */

	if (strlen(ptr) == 0) {
		return 0;
	}

	unsigned char ch = ptr[0];

	/* 1字节 0xxxxxxx */
	if ((unsigned char)(0b10000000 & ch) == (unsigned char)0b0) {
		return 1;
	}

	/* 2字节 110xxxxx 10xxxxxx */
	if ((unsigned char)(0b11100000 & ch) == (unsigned char)0b11000000) {
		return 2;
	}

	/* 3字节 1110xxxx 10xxxxxx 10xxxxxx */
	if ((unsigned char)(0b11110000 & ch)== (unsigned char)0b11100000) {
		return 3;
	}

	/* 4字节 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx */
	if ((unsigned char)(0b11111000 & ch) == (unsigned char)0b11110000) {
		return 4;
	}

	/* 5字节 111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
	if ((unsigned char)(0b11111100 & ch) == (unsigned char)0b11111000) {
		return 5;
	}

	/* 6字节 1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx */
	if ((unsigned char)(0b11111110 & ch) == (unsigned char)0b11111100) {
		return 6;
	}

	/* printf("non UTF8 char\n"); */
	return 1;
}


/* fill buf with the next utf8 character and return its length of bytes */
int next_utf8_char(const char* line, char*buf) {
	memset(buf, 0, 7);
	int off = utf8_offset(line);
	if (off > 0) {
		strncpy(buf, line, off);
	}
	return off;
}

/* length of UTF-8 encoded characters */
int utf8_length(const char* line) {
	int len = 0;
	int start = 0;
	while (true) {
		int off = utf8_offset(line+start);
		if (off > 0) {
			++len;
		} else {
			break;
		}
		start += off;
	}
	return len;
}


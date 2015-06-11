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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../log.h"
#include "../b64.h"

/*
 * Python 2.7.6 (default, Sep  9 2014, 15:04:36)
 * [GCC 4.2.1 Compatible Apple LLVM 6.0 (clang-600.0.39)] on darwin
 * Type "help", "copyright", "credits" or "license" for more information.
 * >>> import base64
 * >>> base64.b64encode('pleasure')
 * 'cGxlYXN1cmU='
 * >>> base64.b64encode('pleasur')
 * 'cGxlYXN1cg=='
 * >>> base64.b64encode('pleasu')
 * 'cGxlYXN1'
 * >>> ^D
 */


void testB64(const char* raw, const char* encoded) {
	char* b64 = b64encode(raw);
	check(strcmp(b64, encoded)==0);
	free(b64);
}


int main(int argc, const char *argv[])
{

	log_init("./debug.log", LOG_CONSOLE);

	testB64("pleasure", "cGxlYXN1cmU=");
	testB64("pleasur",  "cGxlYXN1cg==");
	testB64("pleasu",   "cGxlYXN1");

	return EXIT_SUCCESS;
}












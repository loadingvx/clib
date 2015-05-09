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
 This is a handy string utils set, including string split,split,charset convert
 and etc.
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../log.h"




int main(int argc, const char *argv[])
{

	log_init("debug", LOG_CONSOLE);

	char* pwd  = strdup(getenv("PWD"));
	char* home = strdup(getenv("HOME"));
	char* path = strdup(getenv("PATH"));

	info("Tring to add env-path{HOME, PATH} into trie-tree.\n");

	info("Tring to add env{PWD, HOME, PATH} into trie-tree.\n");
	return EXIT_SUCCESS;
}












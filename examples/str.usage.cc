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

//std-c
#include <stdio.h>
#include <stdlib.h>

#include "../str.h"
#include "../log.h"
#include<string>
#include<vector>

int testStringSplitNormal() {
	const char* p = "this is a simple example";
	std::vector<std::string> words;
	int num = split(p, " ", words);
	for(int i = 0; i<num; i++) {
		printf("%s\n", words[i].c_str());
	}
	check(num == 5);
	return 0;
}

int testStringSplitEmpty() {
	std::vector<std::string> words;
	int num = split("", " ", words);
	check(num == -1);
	check(words.size() == 0);
	
	return 0;
}


int testStringStrip() {
	std::string s = "xxymThis is a simple exampleYYY";
	strip(s, "xymY");
	check(s == "This is a simple example");

	check(strip(s, "") == -1);

	return 0;
}



int main(int argc, const char *argv[])
{
	log_init("debug.log", LOG_CONSOLE);

	testStringSplitEmpty();
	testStringSplitNormal();

	testStringStrip();

	return EXIT_SUCCESS;
}


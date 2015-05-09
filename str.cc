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


#include "str.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif

int split(const std::string &src, const std::string &delimeter, std::vector<std::string> &fields) {
	fields.clear();

	if (src.empty()) {
		notice("Spliting empty string by \"%s\"\n", delimeter.c_str());
		return 0;
	}

	size_t begin = 0;
	size_t end   = 0;

	while ( true ) {
		end = src.find(delimeter, begin);
		if(end == std::string::npos) {
			break;
		}
		fields.push_back(src.substr(begin, end-begin));
		begin = end + delimeter.size();
	}

	fields.push_back(src.substr(begin, end-begin));

	return fields.size();
}


#ifdef __cplusplus
}
#endif



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

#include "boyer_moore.h"
#include "log.h"
#include <map>
#include <vector>
#include <string>
#include <cstring>

#ifdef __cplusplus
extern "C" {
#endif

size_t last_bad_char(const std::string &pattern, const char bad_char) {
	return pattern.rfind(bad_char);
}

int bad_char_offset(const std::string &pattern, const char bad_char){
	size_t found = last_bad_char(pattern, bad_char);
	if (found == std::string::npos) {
		return pattern.size();
	}
	return static_cast<int>(pattern.size() -1 - found);
}

size_t last_longest_suffix(const std::string &pattern,const std::string &good) {
	return pattern.substr(0, pattern.size() - 1).rfind(good);
}

int suffix_offset(const std::string &pattern, const std::vector<std::string>&suffixs) {
	std::vector<std::string>::const_reverse_iterator good = suffixs.rbegin();
	size_t found = last_longest_suffix(pattern, *good);
	if (found == std::string::npos) {
		for (;good != suffixs.rend(); good--) {
			if (pattern.substr(0, good->size()) == *good) {
				return pattern.size() - good->size();
			}
		}
	}
	return found;
}

int step(const std::string& pattern, const char bad_char, const std::vector<std::string> &suffixs) {
	int step_by_suffix = (suffixs.size() == 0 ? 0:suffix_offset(pattern, suffixs));
	int step_by_bdchar = bad_char_offset(pattern, bad_char);
	if (step_by_bdchar > step_by_suffix) {
		info("Step by bad-char offset : %d\n", step_by_bdchar);
		return step_by_bdchar;
	}
	info("Step by suffixs offset : %d\n", step_by_suffix);
	return step_by_suffix;
}


int bmsearch(const char* pattern, const char* src) {

	int plen = strlen(pattern);
	int slen = strlen(src);

	if (plen > slen) {         /* pattern is longer than src */
		return -1;
	}

	std::vector<std::string> suffixs;
	int pos = 0;               /* start at the begining of src */
	while (slen - pos >= plen) {
		info("Txt [%s]\n", src+pos);
		info("Key [%s]\n", pattern);
		for(int j = plen-1; j >=0; j--) {
			if (src[pos+j] == pattern[j]) {
				info("%c==%c, suffix : %s\n", src[pos+j], pattern[j], pattern+j);
				suffixs.push_back(pattern+j);
				if(j == 0) {
					info("Pattern Match!\n");
					return pos;
				}
			} else {
				info("%c!=%c, badchar : %c\n", src[pos+j], pattern[j], src[pos+j]);
				int s = step(pattern, src[pos+j], suffixs);
				pos += s;
				info("step forward %d => %d\n", pos, pos+s);
				suffixs.clear();
				break;
			}
		}
	}
	return -1;
}

#ifdef __cplusplus
}
#endif






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

#ifndef CFG_H
#define CFG_H

#include <string>
#include <vector>
#include <map>


class Config {
	private:
		char* fname;
		long modify_time;
		std::map<std::string, std::string>cache;

		std::string find(const char* section, const char* key);
		bool        file_changed();
		bool        update_cache();
	public:
		std::string string (const char* section, const char* key);
		int         number (const char* section, const char* key);
		bool        boolean(const char* section, const char* key);
	public:
		Config(std::string config_file);
		~Config();
};





#endif



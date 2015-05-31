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

#include "cfg.h"
#include "log.h"
#include "str.h"
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <set>

extern char **environ;

std::string Config::dot(const char* section, const char* key) {
	std::string s = "";
	if (section != NULL && strlen(section) > 0) {
		s = std::string(section) + ".";
	}
	return s+key;
}

bool Config::expand() {

	char **p = environ;
	std::vector<std::string> fields;
	while (*p != NULL)
	{
		split(*p, "=", fields);
		check(fields.size() == 2);
		this->env.insert(std::pair<std::string, std::string>(fields[0], fields[1]));
		p++;
	}

	std::map<std::string, std::string::size_type> need_parse;
	for (std::map<std::string, std::string>::iterator i = this->cache.begin(); i != this->cache.end(); i++) {
		std::string::size_type var_pos = i->second.find("${");
		if (var_pos != std::string::npos) {
			need_parse.insert(std::pair<std::string, std::string::size_type>(i->first, var_pos + 2));
		}
	}

	std::set<std::string> vars;
	std::string v;
	std::map<std::string, std::string::size_type>::iterator it;
	while (need_parse.size() > 0) {
		it = need_parse.begin();
		std::string::size_type end = this->cache[it->first].find("}");
		check(end != std::string::npos);
		v = this->cache[it->first].substr(it->second, end - it->second);
		check(this->cache.find(v) != this->cache.end() || this->env.find(v) != this->env.end());
		check(vars.find(v) == vars.end()); /* fatal: variable defination loop */

		std::map<std::string, std::string::size_type>::iterator ineed;
		for(ineed = it; ineed != need_parse.end(); ineed++) {
			if (this->cache.find(v) != this->cache.end()) {
				replace(this->cache[ineed->first], "${"+v+"}", this->cache[v]);
			}
			if (this->env.find(v) != this->env.end()) {
				replace(this->cache[ineed->first], "${"+v+"}", this->env[v]);
			}
		}
		vars.insert(v);
		for (std::map<std::string, std::string>::iterator i = this->cache.begin(); i != this->cache.end(); i++) {
			if (i->second.find("${") == i->second.npos) {
				need_parse.erase(i->first);
			}
		}
	}
	return true;
}


bool Config::update_cache() {
	if (! this->file_changed()) {
		return true;
	}

	check(this->fname);
	if (access(this->fname, R_OK) != 0) {
		info("accessing(read) %s failure\n", this->fname);
		return "";
	}

	FILE* f = fopen(this->fname, "r");
	if (f == NULL) {
		perror(this->fname);
	}

	char* buf = (char*) malloc(sizeof(char)*1024);
	std::string current_section = "";
	std::vector<std::string> fields;

	ssize_t bytes_cnt = 0;
	size_t size;
	while ((bytes_cnt = getline(&buf, &size, f)) != EOF) {
		std::string line(buf);
		strip(line, " \t\n\r");
		if (line.size() == 0) {
			continue;                    /* ignore empty lines */
		}
		if (line[0] == '#') {
			continue;                    /* ignore comments    */
		}

		size_t n = line.find("#");
		if (n != std::string::npos) {
			line = line.substr(0, n);    /* erase comment tail */
			strip(line, " \t\n\r");
			if (line.size() == 0) {
				continue;
			}
		}

		split(line, "=", fields);
		std::string head = fields[0];
		strip(head, " \t\n\r");
		if(head[0] == '[' && head[head.size() -1 ] == ']') {
			strip(head, "][ \t");
			check(head.size() > 0);
			current_section = head;
			continue;     /* begin of new section   */
		}
		check(fields.size() == 2);
		std::string tail = fields[1];
		strip(tail, " \t\n\r");
		this->cache.insert(std::pair<std::string, std::string>(this->dot(current_section.c_str(), head.c_str()), tail));
	}
	free(buf);
	fclose(f);
	return true;
}

Config::Config(std::string config_file) {
	this->fname = strdup(config_file.c_str());
	this->modify_time = 0;
	this->cache.clear();
	this->update_cache();
	this->expand();
}

Config::~Config() {
	check(this->fname);
	free(this->fname);
	this->fname = NULL;
}

bool Config::file_changed() {
	struct stat status;
	check(this->fname);

	int st = stat(this->fname, &status);
	if (st == -1) {
		perror(this->fname);
		return false;
	}

	if (this->modify_time == 0) {
		this->modify_time = status.st_mtime;
		return true;
	}

	if (this->modify_time != 0 && status.st_mtime > this->modify_time) {
		this->modify_time = status.st_mtime;
		info("Detected file changes!\n");
		return true;
	}

	return false;
}

std::string Config::find(const char* section, const char* key) {
	this->update_cache();
	std::map<std::string, std::string>::iterator icache = this->cache.find(this->dot(section, key));
	if (icache != this->cache.end()) {
		return icache->second;
	}
	return "";
}

std::string Config::string(const char* section, const char* key) {
	return this->find(section, key);
}

int Config::number(const char* section, const char* key) {
	std::string value = this->find(section, key);
	if (value == "") {
		return INT_MAX;
	}
	return atoi(value.c_str());
}

bool Config::boolean(const char* section, const char* key) {
	std::string value = this->find(section, key);
	if (value.size() == 0) {
		return false;
	}
	if (value == "true") {
		return true;
	}
	return false;
}



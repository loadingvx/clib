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

Config::Config(std::string config_file) {
	this->fname = strdup(config_file.c_str());
	this->modify_time = 0;
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
	if (this->modify_time != 0 && status.st_mtime > this->modify_time) {
		this->modify_time = status.st_mtime;
		return true;
	}
	return false;
}


std::string Config::option(const char* section, const char* key) {
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
	int section_status = 0;
	std::vector<std::string> fields;
	if (section == NULL || strlen(section) == 0) {
		section_status = -1;                 /* ignore section */
	}

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
		}

		split(line, "=", fields);
		std::string head = fields[0];
		strip(head, " \t\n\r");
		if (section_status != -1) {
			if(head[0] == '[' && head[head.size() -1 ] == ']') {
				if (section_status == 1) {
					free(buf);
					fclose(f);
					return "";        /* end of section    */
				}
				strip(head, "][ \t");
				if (strcmp(head.c_str(), section) == 0 && section_status == 0) {
					section_status = 1;
					continue;     /* begin of section   */
				}
			}
			if(section_status == 1 && strcmp(head.c_str(), key) == 0) {
				free(buf);
				fclose(f);
				return line;
			}
		} else {
			if(strcmp(head.c_str(), key) == 0) {
				free(buf);
				fclose(f);
				return line;
			}
		}
	}
	free(buf);
	fclose(f);
	return "";
}


bool Config::kv_pair(std::string &line, std::vector<std::string>& fields) {
	split(line, "=", fields);
	check(fields.size() == 2);
	if (fields.size() != 2) {
		info("BadValue Config: %s\n", line.c_str());
		return false;
	}
	return true;
}

std::string Config::get_str(const char* section, const char* key) {
	std::vector<std::string> fields;
	std::string line = this->option(section, key);
	if (line.size() == 0) {
		return "";
	}
	if (!kv_pair(line, fields)) {
		return "";
	}
	std::string value = fields[1];
	strip(value, "\t\r\n ");
	return value;
}

int Config::get_num(const char* section, const char* key) {
	std::vector<std::string> fields;
	std::string line = this->option(section, key);
	if (line.size() == 0) {
		return INT_MAX;
	}
	if (!kv_pair(line, fields)) {
		return INT_MAX;
	}
	std::string value = fields[1];
	strip(value, "\t\r\n ");
	return atoi(value.c_str());
}

bool Config::get_bool(const char* section, const char* key) {
	std::vector<std::string> fields;
	std::string line = this->option(section, key);
	if (line.size() == 0) {
		return false;
	}

	if (!kv_pair(line, fields)) {
		return false;
	}
	std::string value = fields[1];
	strip(value, "\t\r\n ");
	if (value == "true") {
		return true;
	}
	return false;
}



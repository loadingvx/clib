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

#include "log.h"
#include<stdarg.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

#define CON_COLOR       "\033[34m"
#define LOCATION_COLOR  "\033[34;1m"
#define ERRMSG_COLOR    "\033[33m"
#define END_COLOR       "\033[0m"

#define CHECK_FLAG(options, opt) (options&opt)


/* cas lock free */
#define CAS_VALID  0
#define CAS_OCCUPY 1

volatile int __lock = CAS_VALID;

#define cas_try  __sync_bool_compare_and_swap(&__lock, CAS_VALID, CAS_OCCUPY)
#define cas_free __sync_bool_compare_and_swap(&__lock, CAS_OCCUPY, CAS_VALID)

struct log_setting {
	char filename[128];
	int  level;
	int  options;
	int  hour;
	int  min;
	int  day;
	int  cnt;
	int  preck;
} setting;

const char* LEVEL[] = { "INFO", "NOTICE", "WARN", "EROR", "FATAL" };

void log_daily_rotate(int _hour, int _min, int _reqs_precheck) {
	if(_hour < 0 || _hour >= 24 || _min < 0 || _min >= 60) {
		fprintf(stderr, "Invalid Time %d:%d\n", _hour, _min);
		return;
	}
	setting.hour  = _hour;
	setting.min   = _min;
	setting.preck = _reqs_precheck;
}


void _now(char* buf, int len, const char* fmt) {
  check(buf != NULL);
	time_t t=time(NULL);
	strftime (buf, len, fmt, localtime(&t));
}

int logrotate() {
	time_t _t =time(NULL);
	struct tm* t = localtime(&_t);
	if ( setting.day != t->tm_mday &&
		t->tm_min >= setting.min && t->tm_hour >= setting.hour) {
		while(!cas_try);  /* enter critical */
		char * newName = (char*)malloc(sizeof(char)*256);
		memset(newName, 0, 256);
		snprintf(newName, 255, "%s.%d-%d-%d.%d.%d", setting.filename,
			 t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
			 t->tm_hour, t->tm_min);
		if(rename(setting.filename, newName)) {
			printf("checking[%d]\n", t->tm_min);
			perror(newName);
			return -1;
		}
		free(newName);
		setting.day = t->tm_mday;
		while(!cas_free); /* leave critical */
	}
	return 0;
}

void print_and_abort(const char* file, int line, const char* expr) {
	if(! isatty(fileno(stderr))) {
		fprintf(stderr, "[ %s ] failed, see <$vim %s +%d>\n", expr, file, line);
	} else {
		fprintf(stderr, CON_COLOR"[ %s ] failed, see <$vim %s +%d>\n"END_COLOR, expr, file, line);
	}
	exit(-1);
}


void log_init(const char* _filename, int options) {
	memset(&(setting.options), options, 1);
	memset(setting.filename, 0, 127);
	snprintf(setting.filename, 127, "%s", _filename);
	check(setting.options==options);
	setting.day  = -1;
	setting.min  = -1;
	setting.hour = -1;
}

const char* getpromote(int level) {
	switch(level) {
	case LOG_INFO:
		return LEVEL[0];
	case LOG_NOTICE:
		return LEVEL[1];
	case LOG_WARN:
		return LEVEL[2];
	case LOG_ERROR:
		return LEVEL[3];
	case LOG_FATAL:
		return LEVEL[4];
	}
	return NULL; /* should not be here */
}

void get_prefix(char* prefix, int len, const char* file, int line, int level) {
	memset(prefix, 0x0, len);
	char now[20];
	memset(now, 0, 20);
	_now(now, 20, "%Y-%m-%d %H:%M:%S");
	snprintf(prefix, len-1, "[%s] %s [%s +%d] ", now, getpromote(level), file, line);
}


void logger_impl(int level, const char* file, int line, const char* fmt, ...) {
	char prefix[128];
	get_prefix(prefix, 128, file, line, level);

	va_list vars;
	va_start(vars, fmt);
	char _buf[2048];
	memset(_buf,0,2048);

	if (CHECK_FLAG(setting.options, LOG_DAILY_ROTATE) && setting.cnt%setting.preck== 0) {
		logrotate();
	}

	int used = sprintf(_buf, "%s", prefix);
	vsprintf(_buf+used, fmt, vars);
	va_end(vars);

	if (CHECK_FLAG(setting.options, LOG_CONSOLE)) {
		fprintf(stderr, "%s", _buf);
	}

	while(!cas_try); /* enter critical */
	FILE *f = fopen(setting.filename, "a+");
	check(f!=NULL);
	if (f==NULL) {
		perror(setting.filename);
		return;
	}
	fprintf(f, "%s", _buf);
	fclose(f);

	setting.cnt += 1;
	while(!cas_free); /* leave critical */

}

#ifdef __cplusplus
}
#endif



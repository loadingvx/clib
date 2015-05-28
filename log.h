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

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C"{
#endif

#include<stdio.h>
#include<stdarg.h>

/* log behavior */
#define LOG_CONSOLE        0x01
#define LOG_DAILY_ROTATE   0x02


/* logging levels */
#define LOG_INFO     0x01
#define LOG_NOTICE   0x02
#define LOG_WARN     0x04
#define LOG_ERROR    0x08
#define LOG_FATAL    0x10

void log_init(const char* _filename, int options);
void log_daily_rotate(int _hour, int _min, int _reqs_precheck);
void logger_impl(int level, const char* file, int line, const char* fmt, ...);


#ifdef DEBUG
#define info(fmt, ...)   {  \
	logger_impl(LOG_INFO,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define warn(fmt, ...)   {  \
	logger_impl(LOG_WARN,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}


#define notice(fmt, ...)   {  \
	logger_impl(LOG_NOTICE,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}
/* enable debug and checking if DEBUG if defined */
#define debug(fmt, ...)   {  \
	logger_impl(LOG_NOTICE,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

void print_and_abort(const char* file, int line, const char* expr);
#define require(condition) {                 \
	if(!(condition)) {                        \
		print_and_abort(__FILE__, __LINE__, #condition); \
	} \
}

#define check(condition) require(condition)

#else

/* erase all debug/check/info/notice/warn from release */
#define info(...)
#define notice(...)
#define warn(...)
#define debug(...)
#define require(...)
#define check(...)
#endif


#define error(fmt, ...)   {  \
	logger_impl(LOG_ERROR,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#define fatal(fmt, ...)   {  \
	logger_impl(LOG_FATAL,  __FILE__, __LINE__, fmt, ##__VA_ARGS__); \
}

#ifdef __cplusplus
}
#endif

#endif



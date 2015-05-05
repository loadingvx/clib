//
//  log.h
//
//
//  Created by liang chengming on 2015-05-04.
//  Copyright (c) 2013年 liang chengming. All rights reserved.
//
//



#include<stdio.h>
#include<stdarg.h>

/* log behavior */
#define LOG_SYSLOG        0x01
#define LOG_CONSOLE       0x02
#define LOG_DAILY_ROTATE  0x04
#define LOG_PREFIX_SRC    0x08
#define LOG_PREFIX_PID    0x10
#define LOG_PREFIX_UID    0x20


/* logging levels */
#define LOG_INFO     0x01
#define LOG_NOTICE   0x02
#define LOG_WARN     LOG_NOTICE
#define LOG_ERROR    0x04
#define LOG_FATAL    LOG_ERROR

void log_init(const char* _filename, int options);
void log_daily_rotate(int _hour, int _min);
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





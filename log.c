#include "log.h"
#include<stdarg.h>
#include<time.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>


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
} setting;

char* LEVEL[] = { "INFO", "WARN", "EROR" };

void log_daily_rotate(int _hour, int _min) {
	if(_hour < 0 || _hour >= 24 || _min < 0 || _min >= 60) {
		fprintf(stderr, "Invalid Time %d:%d\n", _hour, _min);
		return;
	}
	setting.hour = _hour;
	setting.min  = _min;
}


inline void _now(char* buf, int len, const char* fmt) {
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

void get_prefix(char* prefix, int len, const char* file, int line) {
	memset(prefix, 0x0, len);
	char now[20];
	_now(now, 19, "%Y-%m-%d %H:%M:%S");
	snprintf(prefix, len-1, "[%s] INFO [%s +%d] ", now, file, line);
}


void logger_impl(int level, const char* file, int line, const char* fmt, ...) {
	char prefix[128];
	get_prefix(prefix, 128, file, line);

	va_list vars;
	va_start(vars, fmt);
	char* buf = (char*)calloc(1, sizeof(char)*4096);

	if (CHECK_FLAG(setting.options, LOG_DAILY_ROTATE) && setting.cnt%20000 == 0) {
		logrotate();
	}

	while(!cas_try); /* enter critical */

	FILE *f = fopen(setting.filename, "a+");
	if (f==NULL) {
		perror(setting.filename);
		return;
	}
	int used = sprintf(buf, "%s", prefix);
	vsprintf(buf+used, fmt, vars);
	va_end(vars);

	fprintf(f, "%s", buf);

	if (CHECK_FLAG(setting.options, LOG_CONSOLE)) {
		fprintf(stderr, "%s", buf);
	}
	setting.cnt += 1;

	while(!cas_free); /* leave critical */

	free(buf);
	fclose(f);
	
}




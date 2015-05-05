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


char* LEVEL[] = { "INFO", "WARN", "EROR" };



void print_and_abort(const char* file, int line, const char* expr) {
	if(! isatty(fileno(stderr))) {
		fprintf(stderr, "[ %s ] failed, see <$vim %s +%d>\n", expr, file, line);
	} else {
		fprintf(stderr, CON_COLOR"[ %s ] failed, see <$vim %s +%d>\n"END_COLOR, expr, file, line);
	}
	exit(-1);
}


struct log_setting {
	char filename[128];
	int  level;
	int  options;
} setting;


void log_init(const char* _filename, int options) {
	memset(&(setting.options), options, 1);
	memset(setting.filename, 0, 127);
	snprintf(setting.filename, 127, "%s", _filename);
	check(setting.options==options);
}

void get_prefix(char* prefix, int len, const char* file, int line) {
	memset(prefix, 0x0, len);
	char now[20];
	time_t t=time(NULL);
	strftime (now, 20, "%Y-%m-%d %H:%M:%S", localtime(&t));
	snprintf(prefix, len-1, "[%s] INFO [%s +%d] ", now, file, line);
}


void logger_impl(int level, const char* file, int line, const char* fmt, ...) {
	char prefix[128];
	get_prefix(prefix, 128, file, line);

	FILE *f = fopen(setting.filename, "a+");
	if (f==NULL) {
		perror(setting.filename);
		return;
	}


	va_list vars;
	va_start(vars, fmt);
	char* buf = (char*)calloc(1, sizeof(char)*4096);

	while(!cas_try); /* enter critical */

	int used = sprintf(buf, "%s", prefix);
	vsprintf(buf+used, fmt, vars);
	va_end(vars);

	fprintf(f, "%s", buf);

	if (CHECK_FLAG(setting.options, LOG_ENABLE_CONSOLE)) {
		fprintf(stderr, "%s", buf);
	}

	while(!cas_free); /* leave critical */

	free(buf);
	fclose(f);
	
}




#include "../log.h"
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define TIMES 100000

void writelog(){
	int thread = (int)pthread_self();
	for(int i = 0; i < TIMES; i++) {
		info("thread_id=%d\n", thread);
		notice("thread_id=%d\n", thread);
		warn("thread_id=%d\n", thread);
	}
}


void test_multi_thread(int thread_num, void (*p)(void) ) {
	pthread_t *id = (pthread_t *)calloc(1, sizeof(pthread_t) * thread_num);

	for(int i = 0; i < thread_num; i++) {
		int ret = pthread_create(id+i, NULL, (void *)p, NULL);
	}

	for(int i = 0; i < thread_num; i++) {
		pthread_join(id[i],NULL);
	}

	free(id);
}


int main(int argc, const char *argv[])
{
	log_init("./debug.log", LOG_DAILY_ROTATE);
	log_daily_rotate(0, atoi(argv[1]), 10000);

	check(1==1);
	check(!strcmp("f", "f")); /* checking a 'boolean' value */

	test_multi_thread(20, writelog);

	return EXIT_SUCCESS;
}

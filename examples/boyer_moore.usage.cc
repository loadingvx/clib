
#include <stdio.h>
#include <stdlib.h>
#include "../log.h"
#include "../boyer_moore.h"

int main(int argc, const char *argv[])
{
	log_init("bm.log", LOG_CONSOLE);
	const char* src = "This is a simple example";
	const char* pattern = "example";
	int found = bmsearch(pattern, src);
	if(found != -1) {
		printf("found = %d\n", found);
	}

	return EXIT_SUCCESS;
}


//std-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../levenshtein.h"
#include "../log.h"


int main(int argc, const char *argv[])
{
	check(argc >= 3);

	int distance = levenshtein(argv[1], argv[2]);
	printf("edit-distance <%s,%s> = %d\n", argv[1], argv[2], distance);

	return EXIT_SUCCESS;
}


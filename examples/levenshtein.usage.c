//std-c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../levenshtein.h"
#include "../log.h"


int main(int argc, const char *argv[])
{
	check(argc >= 3);

	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);

	int distance = levenshtein(argv[1], len1, argv[2], len2);
	printf("edit-distance <%s,%s> = %d\n", argv[1], argv[2], distance);

	return EXIT_SUCCESS;
}


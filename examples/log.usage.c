#include "../log.h"
#include <stdlib.h>


int main(int argc, const char *argv[])
{
	log_init("./debug.log", LOG_ENABLE_CONSOLE);

	info("%d, %s\n", 100, "info msg here!");

	check(1==1);

	int a = 0;
	int b = 1;
	require(a == b);

	return EXIT_SUCCESS;
}

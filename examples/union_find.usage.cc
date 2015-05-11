//std-c
#include <stdio.h>
#include <stdlib.h>
#include "../union_find.h"
#include "../log.h"

int testDisjointPOJ1611() {

	/*
	 * 100 students, students in the same group stay together frequently.
	 * student number0 now infected SARS.
	 *
	 * Find out all suspects now !!
	 *
	 * group1 : 1 2
	 * group2 : 10 13 11 12 14
	 * group3 : 0 1
	 * group4 : 99 2
	*/

	struct UFS* disjoint = ufs_open(100);

	ufs_union(disjoint, 1,  2);  /* group 1 */
	ufs_union(disjoint, 0, 1);   /* group 3 */
	ufs_union(disjoint, 99, 2);  /* group 4 */

	ufs_union(disjoint, 13, 10); /* group 2 */
	ufs_union(disjoint, 11, 10); /* group 2 */
	ufs_union(disjoint, 12, 10); /* group 2 */
	ufs_union(disjoint, 14, 10); /* group 2 */

	for (int i = 0; i < 100; i++) {
		if (ufs_connected(disjoint, 0, i)) {
			printf("suspect stduct: id = %d\n", i);
		}
	}

	ufs_close(disjoint);

	return 0;
}



int main(int argc, const char *argv[])
{

	log_init("debug.log", LOG_CONSOLE);
	testDisjointPOJ1611();

	return EXIT_SUCCESS;
}





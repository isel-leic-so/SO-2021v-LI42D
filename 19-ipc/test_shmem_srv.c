#include <stdlib.h>
#include <stdio.h>
#include "shmem.h"

#define SHMEM_NAME "/isel.so.shmem.test"
#define SHMEM_SIZE 4096

int main() {
	puts(":: STARTED ::");
	
	puts("Creating and mapping a shared memory object");
	void * mem = shmem_create(SHMEM_NAME, SHMEM_SIZE);
	if (!mem) {
		exit(1);
	}

	for (;;) {
		puts("** READY: press ENTER to read **");
		getchar();

		int val = *(int *)mem;

		printf("SHARED: %d\n", val);
		if (val == 0) break;
	}
	
	puts("** DONE: press ENTER to exit **");
	getchar();
	
	shmem_destroy(SHMEM_NAME, mem, SHMEM_SIZE);
	return 0;
}
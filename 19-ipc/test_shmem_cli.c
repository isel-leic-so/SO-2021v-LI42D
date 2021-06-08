#include <stdlib.h>
#include <stdio.h>
#include "shmem.h"

#define SHMEM_NAME "/isel.so.shmem.test"
#define SHMEM_SIZE 4096

int main(int argc, char * argv[]) {
	puts(":: STARTED ::");
	
	puts("Opening and mapping a shared memory object");
	void * mem = shmem_open(SHMEM_NAME);
	if (!mem) {
		exit(1);
	}

	puts("** READY: press ENTER to write **");
	getchar();

	int val = ((argc == 1) ? 0 : atoi(argv[1]));
	printf("SHARING: %d\n", val);
	*(int *)mem = val;

	puts("** DONE: press ENTER to exit **");
	getchar();
	
	shmem_close(mem, SHMEM_SIZE);
	return 0;
}

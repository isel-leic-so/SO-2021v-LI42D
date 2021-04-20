#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define SHMEM_NAME "/isel.so.shm.example01"
#define SHMEM_SIZE 4096

int main() {
	puts(":: STARTED ::");
	
	printf("> Creating a shared memory object named \"%s\"\n", SHMEM_NAME);
	int fd = shm_open(SHMEM_NAME, O_CREAT | O_EXCL | O_RDWR, 0666);
	if (fd == -1) {
		perror(">> Failed to create shared memory");
		return 1;
	}

	printf("> Setting the size of the shared memory object to %d\n", SHMEM_SIZE);
	if (ftruncate(fd, SHMEM_SIZE) == -1) {
		perror(">> Failed to set the size of shared memory");
		close(fd);
		shm_unlink(SHMEM_NAME);
		return 2;
	}

	puts("> Mapping the shared memory object");
	void * ptr = mmap(NULL, SHMEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror(">> Failed to map the shared memory");
		close(fd);
		shm_unlink(SHMEM_NAME);
		return 3;
	}
	close(fd);
	printf("> Shared memory object mapped at %p\n", ptr);

	puts("> Writing to shared memory");
	strcpy(ptr, "ISEL - LEIC - SO");

	puts("> Done");
	munmap(ptr, SHMEM_SIZE);

	return 0;
}

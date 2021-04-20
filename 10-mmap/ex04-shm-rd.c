#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

#define SHMEM_NAME "/isel.so.shm.example01"

int main() {
	puts(":: STARTED ::");
	
	printf("> Opening the shared memory object named \"%s\"\n", SHMEM_NAME);
	int fd = shm_open(SHMEM_NAME, O_RDWR, 0666);
	if (fd == -1) {
		perror(">> Failed to open shared memory");
		return 1;
	}

	puts("> Getting the size of the shared memory object");
	struct stat statbuf;
	if (fstat(fd, &statbuf) == -1) {  // MIGHT FAIL if ftruncate has not been called yet
		perror(">> Failed to get the size of shared memory");
		close(fd);
		return 2;
	}
	size_t size = statbuf.st_size;
	printf("> The size of the shared memory object is %lu\n", size);

	puts("> Mapping the shared memory object");
	void * ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (ptr == MAP_FAILED) {
		perror(">> Failed to map the shared memory");
		close(fd);
		return 2;
	}
	close(fd);
	printf("> Shared memory object mapped at %p\n", ptr);

	puts("> Reading from shared memory");
	puts(ptr);

	puts("> Done");
	munmap(ptr, size);
	
	return 0;
}

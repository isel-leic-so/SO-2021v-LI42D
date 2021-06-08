#include "shmem.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static void * shmem_internal_map(int fd, size_t size) {
	
	// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	void * base = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if (base == MAP_FAILED) {
		return NULL;
	}
	printf("mapped %lu\n", size);
	return base;
}

void * shmem_create(const char *name, size_t size) {
	
	// int shm_open(const char *name, int oflag, mode_t mode);
	int fd = shm_open(name, O_CREAT | O_EXCL | O_RDWR, 0666);
	if (fd == -1) {
		perror("Failed to create shared memory");
		return NULL;
	}

	// int ftruncate(int fd, off_t length);
	if (ftruncate(fd, size) == -1) {
		perror("Failed to set the size of shared memory");
		close(fd);
		shm_unlink(name);
		return NULL;
	}
	
	// void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);
	void * base = shmem_internal_map(fd, size);
	if (base == NULL) {
		perror("Failed to map shared memory");
		shm_unlink(name);
	}
	
	close(fd);
	return base;
}

void shmem_destroy(const char * name, void *base, size_t size) {
	// int munmap(void *addr, size_t length);
	munmap(base, size);
	shm_unlink(name);
}

void * shmem_open(const char *name) {
	
	// int shm_open(const char *name, int oflag, mode_t mode);
	int fd = shm_open(name, O_RDWR, 0666);
	if (fd == -1) {
		perror("Failed to open shared memory");
		return NULL;
	}

	// int fstat(int fd, struct stat *buf);
	struct stat statbuf;
	if (fstat(fd, &statbuf) == -1) {  // MIGHT FAIL if ftruncate has not been called yet
		perror("Failed to get the size of shared memory");
		close(fd);
		return NULL;
	}

	void * base = shmem_internal_map(fd, statbuf.st_size);
	
	close(fd);
	return base;
}

void shmem_close(void *base, size_t size) {
	// int munmap(void *addr, size_t length);
	munmap(base, size);
}

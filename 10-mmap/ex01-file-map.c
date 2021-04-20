#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char * argv[]) {
	if (argc < 2) {
		fprintf(stderr, "use %s file\n", argv[0]);
		return 1;
	}
	
	int fd = open(argv[1], O_RDWR);
	if (fd == -1) {
		perror("Failed to open file");
		return 2;
	}
	
	void * ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	close(fd);
	
	printf("> Memory mapped at 0x%p\n", ptr);
	
	puts(ptr);

    puts("---- ----");
	
	if (argc > 2) {
		memcpy(ptr, argv[2], strlen(argv[2]));
	}

	puts(ptr);

	munmap(ptr, 4096);
	
	return 0;
}

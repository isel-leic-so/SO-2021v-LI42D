#include <stdio.h>
#include <unistd.h>

// RUN:
//   ./prog2
//
int main() {
	write(1, "prog2 running\n", 14);
	
	// int nfd = dup(1);  // v1
	int nfd = dup2(1, 88);
	printf("nfd: %d\n", nfd);
	
	// int res = write(3, "Hello, world!\n", 14);  // v1
	int res = write(88, "Hello, world!\n", 14);
	if (res == -1) {
		perror("write failed");
	} else {
		puts("prog2 successful");
	}
	
	return 0;
}

#include <stdio.h>
#include <unistd.h>

// RUN:
//   ./prog1
//   ./prog1 3> abc.txt
//
int main() {
	write(1, "prog1 running\n", 14);
	
	int res = write(3, "Hello, world!\n", 14);
	if (res == -1) {
		perror("write failed");
	} else {
		puts("prog1 successful");
	}

	return 0;
}

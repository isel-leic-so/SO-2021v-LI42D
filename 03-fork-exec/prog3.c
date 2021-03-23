#include <stdio.h>
#include <unistd.h>

int main() {
	puts("prog3 running");
	
	printf("PARENT: pid=%d\n", getpid());
	puts("PARENT: fork");
	
	pid_t pid = fork();
	if (pid == 0) {
		printf("CHILD: pid=%d; ppid=%d\n", getpid(), getppid());
	} else {
		printf("PARENT: pid=%d; child=%d\n", getpid(), pid);
	}
	
	puts("SOME MESSAGE");
	
	return 0;
}
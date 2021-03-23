#include <stdio.h>
#include <unistd.h>

int value = 88;

int main() {
	puts("prog4 running");
	
	printf("PARENT: pid=%d\n", getpid());
	puts("PARENT: fork");
	
	pid_t pid = fork();
	if (pid == 0) {
		printf("CHILD: pid=%d; ppid=%d\n", getpid(), getppid());
		value += 1000;
	} else {
		printf("PARENT: pid=%d; child=%d\n", getpid(), pid);
	}
	
	puts("SOME MESSAGE");
	
	for (int i = 0; i < 10; ++i) {
		printf("[%d] value at %p is %d\n", getpid(), &value, value);
		value += 1;
		sleep(1);
	}
	
	return 0;
}
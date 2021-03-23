#include <stdio.h>
#include <unistd.h>

int main() {
	puts("prog5 running");
	
	printf("PARENT: pid=%d\n", getpid());
	puts("PARENT: fork");
	
	pid_t pid = fork();
	if (pid == 0) {
		printf("CHILD: pid=%d; ppid=%d\n", getpid(), getppid());
		
		puts("CHILD: exec");
		execlp("./prog2", "prog2", NULL);
		
		puts("SHOULD NEVER GET HERE!");
	} else {
		printf("PARENT: pid=%d; child=%d\n", getpid(), pid);
		
		// should call waitpid
	}
	
	puts("SOME MESSAGE");
	
	return 0;
}

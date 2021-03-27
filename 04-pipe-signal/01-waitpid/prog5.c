#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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
		
		exit(1);
		//sleep(60);
		
		int res;
		waitpid(pid, &res, 0);
		
		printf("PARENT: child process %d terminated with %d\n", pid, res);
		if (WIFEXITED(res)) {
			printf("PARENT: child returned %d\n", WEXITSTATUS(res));
		} else {
			printf("PARENT: child terminated abnormally\n");
		}
	}
	
	puts("SOME MESSAGE");
	
	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	puts("pipe1 running");
	printf("PARENT: pid=%d\n", getpid());

	int pipefd[2];
	pipe(pipefd);

	puts("PARENT: fork");
	pid_t pid = fork();
	if (pid == 0) {
		printf("CHILD: pid=%d; ppid=%d\n", getpid(), getppid());

		close(pipefd[0]); // fechar a ponta que não vai usar
		
		sleep(10);
		
		write(pipefd[1], "Hello, parent!", 15);

	} else {
		printf("PARENT: pid=%d; child=%d\n", getpid(), pid);

		close(pipefd[1]); // fechar a ponta que não vai usar

		printf("PARENT: waiting for message from child\n");
		
		char msg[15];
		read(pipefd[0], msg, 15);
		
		printf("PARENT: child sent message \"%s\"\n", msg);
		
		int res;
		waitpid(pid, &res, 0);
		
		printf("PARENT: child process %d terminated with %d\n", pid, res);
		if (WIFEXITED(res)) {
			printf("PARENT: child returned %d\n", WEXITSTATUS(res));
		} else {
			printf("PARENT: child terminated abnormally\n");
		}
	}
	
	return 0;
}

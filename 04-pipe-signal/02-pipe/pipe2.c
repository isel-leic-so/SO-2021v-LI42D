#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	puts("pipe2 running");
	printf("PARENT: pid=%d\n", getpid());

	int pipefd[2];
	pipe(pipefd);

	puts("PARENT: fork");
	pid_t pid = fork();
	if (pid == 0) {
		printf("CHILD: pid=%d; ppid=%d\n", getpid(), getppid());

		close(pipefd[0]); // fechar a ponta que não vai usar
		
		dup2(pipefd[1], 1); // substituir standard output por pipe
		
		close(pipefd[1]);  // fechar a ponta de escrita (que foi duplicada)
		
		printf("Hello, parent!\n");

	} else {
		printf("PARENT: pid=%d; child=%d\n", getpid(), pid);

		close(pipefd[1]); // fechar a ponta que não vai usar

		dup2(pipefd[0], 0);
		close(pipefd[0]);  // fechar a ponta de leitura (que foi duplicada)

		printf("PARENT: waiting for messages from child\n");
		
		char msg[256];
		while (fgets(msg, 256, stdin)) {
			size_t len = strlen(msg);
			if (msg[len - 1] == '\n') {
				msg[len - 1] = 0;
			}
			printf("PARENT: child sent message \"%s\"\n", msg);
		}

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

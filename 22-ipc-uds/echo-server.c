#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <pthread.h>

#define BUFSIZE 1024

void terror(char *msg) {
	perror(msg);
	pthread_exit((void *)(intptr_t)1);
}

void * process_connection(void * ptr) {
	int conn_fd = (int)(intptr_t)ptr;

	char buf[BUFSIZE];
	int len;
	
	for (;;) {
		len = read(conn_fd, buf, BUFSIZE);
		if (len < 0) {
			terror("ERROR reading from socket");
		}
		buf[len] = 0;
		printf("server received %d bytes: %s\n", len, buf);
		
		if (len == 0 || (strncmp(buf, "quit", 4) == 0 && buf[4] < ' ')) {
			close(conn_fd);
			break;
		}

		len = write(conn_fd, buf, strlen(buf)+1);
		if (len < 0) { 
			terror("ERROR writing to socket");
		}
		
		if (strncmp(buf, "stop", 4) == 0 && buf[4] < ' ') {
			puts(":: EXIT ::");
			exit(0);
		}
	}

	return NULL;	
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char * argv[]) {
	if (argc != 2) {
		fprintf(stderr, "use: %s socket_name\n", argv[0]);
		exit(1);
	}
	
	puts(":: START ::");
	
	int main_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (main_fd < 0) {
		error("ERROR creating socket");
	}
	
	struct sockaddr_un srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	sprintf(srv_addr.sun_path, "/tmp/%s", argv[1]); // should check max length
	unlink(srv_addr.sun_path);  // just in case...

	if (bind(main_fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
		close(main_fd);
		error("ERROR on binding");
	}
	
	if (listen(main_fd, 5) < 0) { 
		close(main_fd);
		unlink(srv_addr.sun_path);
		error("ERROR on listen");
	}

	puts(":: LISTENING ::");
	
	for (;;) {

		struct sockaddr_un cli_addr;
		unsigned int cli_addr_len = sizeof cli_addr;

		int conn_fd = accept(main_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		if (conn_fd < 0) {
			close(main_fd);
			unlink(srv_addr.sun_path);
			error("ERROR on accept");
		}

		printf("server established connection with client\n");

		pthread_t thread;
		pthread_create(&thread, NULL, process_connection, (void *)(intptr_t)conn_fd);
		pthread_detach(thread);
	}
	
	close(main_fd);
	unlink(srv_addr.sun_path);
	
	puts(":: END ::");
	return 0;
}

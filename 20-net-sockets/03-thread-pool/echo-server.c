#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // inet_ntop

#include "thread_pool.h"

#define BUFSIZE 1024
#define NTHREADS 8

void terror(char *msg) {
	perror(msg);
	exit(1);
}

void process_connection(void * ptr) {
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

		len = write(conn_fd, buf, strlen(buf));
		if (len < 0) { 
			terror("ERROR writing to socket");
		}
		
		if (strncmp(buf, "quit", 4) == 0 && buf[4] < ' ') {
			close(conn_fd);
			break;
		}
		
		if (strncmp(buf, "stop", 4) == 0 && buf[4] < ' ') {
			puts(":: EXIT ::");
			exit(0);
		}
	}
}

void error(char *msg) {
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[]) {

	if (argc != 2) {
		fprintf(stderr, "use: %s <port>\n", argv[0]);
		exit(1);
	}
	int port = atoi(argv[1]);

	puts(":: START ::");

	thread_pool_t tpool;
	thread_pool_init(&tpool, NTHREADS);

	int main_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (main_fd < 0) {
		error("ERROR opening socket");
	}

	// Avoid "ERROR on binding: Address already in use". 
	int optval = 1;
	setsockopt(main_fd, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval , sizeof(int));

	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	srv_addr.sin_port = htons((unsigned short)port);

	if (bind(main_fd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
		error("ERROR on binding");
	}

	if (listen(main_fd, 5) < 0) { 
		error("ERROR on listen");
	}
	
	for (;;) {

		struct sockaddr_in cli_addr;
		unsigned int cli_addr_len = sizeof cli_addr;

		int conn_fd = accept(main_fd, (struct sockaddr *)&cli_addr, &cli_addr_len);
		if (conn_fd < 0) {
			error("ERROR on accept");
		}

		char cli_addr_str[INET_ADDRSTRLEN];
		if (!inet_ntop(AF_INET, &(cli_addr.sin_addr), cli_addr_str, INET_ADDRSTRLEN)) {
			error("ERROR on inet_ntoa\n");
		}

		printf("server established connection with %s at port %d\n", cli_addr_str, ntohs(cli_addr.sin_port));

		thread_pool_submit(&tpool, process_connection, (void *)(intptr_t)conn_fd);
	}
	
	close(main_fd);
	thread_pool_cleanup(&tpool);

	puts(":: END ::");
	return 0;
}

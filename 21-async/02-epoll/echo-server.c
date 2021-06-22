#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>  // inet_ntop

#include <sys/epoll.h>

#define BUFSIZE 1024
#define INFTIM -1

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

	const int MAX_CLIENTS = FOPEN_MAX;

	int events_fd = epoll_create(MAX_CLIENTS);
	if (events_fd == -1) {
		close(main_fd);
		error("ERROR on epoll_create");
	}

	struct epoll_event ev;
	
	ev.data.fd = 0;
	ev.events = EPOLLIN;
	if (epoll_ctl(events_fd, EPOLL_CTL_ADD, 0, &ev) == -1) {
		close(main_fd);
		close(events_fd);
		error("ERROR on epoll_ctl for stdin");;
	}

	ev.data.fd = main_fd;
	ev.events = EPOLLIN;
	if (epoll_ctl(events_fd, EPOLL_CTL_ADD, main_fd, &ev) == -1) {
		close(main_fd);
		close(events_fd);
		error("ERROR on epoll_ctl for main_fd");;
	}
	
	struct epoll_event events[MAX_CLIENTS];
	int nevents = 2;

	char buf[BUFSIZE];
	int len;	

	for (;;) {
		int nready = epoll_wait(events_fd, events, nevents, INFTIM);
		printf("there are %d ready descriptors\n", nready);
		
		for (int i = 0; i < nready; ++i) {
			struct epoll_event * pev = &events[i];
			
			if (pev->data.fd == 0 && pev->events & EPOLLIN) {
				puts("** exiting **");
				goto end;  // goto is usually considered harmful
			} else if (pev->data.fd == main_fd && pev->events & EPOLLIN) {
				puts("** new connection **");

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
			
				if (nevents == MAX_CLIENTS) {
					puts("** maximum clients exceeded");
					close(conn_fd);
				} else {
					ev.data.fd = conn_fd;
					ev.events = EPOLLIN;
					if (epoll_ctl(events_fd, EPOLL_CTL_ADD, conn_fd, &ev) == -1) {
						close(conn_fd);
						error("ERROR on epoll_ctl for conn_fd");;
					} else {
						nevents++;
					}
				}
				
			} else if (pev->events & (EPOLLIN | EPOLLERR)) {
				int conn_fd = pev->data.fd;
				printf("** event occurred on client %d **\n", conn_fd);

				len = read(conn_fd, buf, BUFSIZE);
				if (len < 0) {
					close(conn_fd);
					error("ERROR reading from socket");
				}

				buf[len] = 0;
				printf("server received %d bytes: %s\n", len, buf);
				
				if (len > 0) {
					int wlen = write(conn_fd, buf, len);
					if (wlen < 0) { 
						error("ERROR writing to socket");
					}
				}

				if (len == 0 || (strncmp(buf, "quit", 4) == 0 && buf[4] < ' ')) {
					printf("** client %d closing **\n", conn_fd);
				
					ev.data.fd = conn_fd;
					ev.events = EPOLLIN;
					if (epoll_ctl(events_fd, EPOLL_CTL_DEL, conn_fd, &ev) == -1) {
						close(main_fd);
						close(events_fd);
						error("ERROR on epoll_ctl for conn_fd");
					}
					
					close(conn_fd);
				}
			}
		}
	}
end:
	puts("** closing **");
	close(events_fd);
	close(main_fd);

	puts(":: END ::");
	return 0;
}

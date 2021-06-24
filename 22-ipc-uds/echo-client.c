#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>

#define LINE_LEN 256

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
	
	int conn_fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (conn_fd < 0) {
		error("ERROR creating socket");
	}

	struct sockaddr_un srv_addr;
	memset(&srv_addr, 0, sizeof(srv_addr));
	srv_addr.sun_family = AF_UNIX;
	sprintf(srv_addr.sun_path, "/tmp/%s", argv[1]); // should check max length

	if (connect(conn_fd, (struct sockaddr *)&srv_addr, sizeof (srv_addr)) < 0) {
		error("ERROR connecting socket");
	} 
	
	puts(":: CONNECTED ::");

	char line[LINE_LEN+1];
	size_t len;

	char data[LINE_LEN+1];
	size_t dlen;

	for (;;) {
		if (!fgets(line, LINE_LEN, stdin)) {
			error("ERROR reading stdin");
		}
		len = strlen(line);
		while (len > 0 && (line[len-1] == '\r' || line[len-1] == '\n')) {
			line[--len] = 0;
		}

		if (write(conn_fd, line, len+1) < 0) {
			error("ERROR writing to socket");
		}
		
		dlen = read(conn_fd, data, LINE_LEN);

		if (dlen == 0) {
			close(conn_fd);
			break;
		}

		if (dlen < 0) {
			error("ERROR reading from socket");
		}

		data[dlen] = 0;
		puts(data);
	}

	close(conn_fd);
	puts(":: END ::");
	return 0;
}

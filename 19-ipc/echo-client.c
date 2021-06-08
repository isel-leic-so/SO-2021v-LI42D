#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "echo-service.h"

int main() {
	puts(":: STARTED ::");

	pid_t pid = getpid();

	const size_t PID_IDX = strlen(ECHO_SERVICE_CLIENT_ID_BASE);
	char resp_ch_name[PID_IDX + PID_MAX_LEN + 1];
	sprintf(resp_ch_name, "%s%d", ECHO_SERVICE_CLIENT_ID_BASE, pid);
	printf("++ Creating response channel %s ++\n", resp_ch_name);
	memch_t * rmc = mc_create(sizeof (echomsg_t), 1, resp_ch_name);
	if (!rmc) {
		exit(1);
	}

	puts("++ Connecting... ++");
	memch_t * smc = mc_open(ECHO_SERVICE_SERVER_ID);
	if (!smc) {
		mc_destroy(rmc);
		exit(2);
	}
	puts("++ Connected ++");

	echomsg_t smsg = { .sender = pid };
	echomsg_t rmsg;
	for (;;) {
		printf("Message: ");

		char * line = fgets(smsg.msg, sizeof smsg.msg, stdin);
		if (!line) {
			perror("\nFailed to read line");
			break;
		}
		line[strlen(line) - 1] = 0;

		if (line[0] == 0) {
			puts("++ Closing ++");
			break;
		}

		printf("++ Sending message: \"%s\" ++\n", smsg.msg);
		mc_put(smc, &smsg);

		puts("++ Waiting for reply... ++");
		mc_get(rmc, &rmsg);

		printf("++ Received message: \"%s\" (from %d)++\n", rmsg.msg, rmsg.sender);
	}
	
	puts("++ Cleanup ++");
	mc_close(smc);
	mc_destroy(rmc);

	puts(":: END ::");
	return 0;
}

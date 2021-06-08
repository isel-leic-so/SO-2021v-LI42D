#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "echo-service.h"

int main() {
	puts(":: STARTED ::");

	memch_t * smc = mc_create(sizeof (echomsg_t), 8, ECHO_SERVICE_SERVER_ID);
	if (!smc) {
		exit(1);
	}

	pid_t pid = getpid();

	const size_t PID_IDX = strlen(ECHO_SERVICE_CLIENT_ID_BASE);
	char resp_ch_name[PID_IDX + PID_MAX_LEN + 1];
	strcpy(resp_ch_name, ECHO_SERVICE_CLIENT_ID_BASE);

	echomsg_t msg;
	for (;;) {
		puts("++ Waiting for messages ++");
		mc_get(smc, &msg);

		if (msg.sender == 0 && msg.msg[0] == '\0') {
			puts("++ Stopping ++");
			break;
		}
		
		sprintf(&resp_ch_name[PID_IDX], "%d", msg.sender);

		printf("MSG[%d]: \"%s\" (reply to: %s)\n", msg.sender, msg.msg, resp_ch_name);
		
		memch_t * rmc = mc_open(resp_ch_name);
		if (rmc) {
			msg.sender = pid;
			mc_put(rmc, &msg);
			mc_close(rmc);
		}
	}

	puts("++ Cleanup ++");
	mc_destroy(smc);

	puts(":: END ::");
	return 0;
}	

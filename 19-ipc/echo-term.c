#include <stdio.h>

#include "echo-service.h"

int main() {
	puts(":: STARTED ::");

	puts("++ Connecting... ++");
	memch_t * smc = mc_open(ECHO_SERVICE_SERVER_ID);
	if (!smc) {
		exit(1);
	}
	puts("++ Connected ++");

	puts("++ Sending termination message ++");
	echomsg_t tmsg = { .sender = 0, .msg = "" };
	mc_put(smc, &tmsg);
	
	puts("++ Cleanup ++");
	mc_close(smc);
	
	puts(":: END ::");
	return 0;
}

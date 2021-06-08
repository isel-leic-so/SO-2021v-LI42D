#include <stdlib.h>
#include <stdio.h>
#include "memchannel.h"

#define MC_NAME "/isel.so.memch.test"

int main(int argc, char * argv[]) {
	memch_t * mc = mc_open(MC_NAME);

	int val = ((argc == 1) ? 0 : atoi(argv[1]));
	int n   = ((argc <= 2) ? 1 : atoi(argv[2]));

	for (int i = 0; i < n; ++i) {
		mc_put(mc, &val);
	}

	mc_close(mc);
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include "memchannel.h"

#define MC_NAME "/isel.so.memch.test"

#define MC_CAPACITY 8
#define MC_ELEMSIZE (sizeof (int))

int main() {
	memch_t * mc = mc_create(MC_ELEMSIZE, MC_CAPACITY, MC_NAME);

	for (;;) {
		int val;
		mc_get(mc, &val);
		
		printf("SHARED: %d\n", val);
		
		if (val == 0) break;
	}
	
	mc_destroy(mc);
	return 0;
}

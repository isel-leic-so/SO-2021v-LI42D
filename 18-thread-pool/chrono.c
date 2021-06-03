#include <unistd.h>
#include <time.h>
#include "chrono.h"

chrono_t chrono_start() {
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	return ts;
}

unsigned long chrono_nanos(chrono_t c) {
	unsigned long nanos;
	struct timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	
	if (ts.tv_sec == c.tv_sec) {
		nanos = ts.tv_nsec - c.tv_nsec;
	} else {
		nanos = (
			(1000000000 - c.tv_nsec) +
			(ts.tv_sec - c.tv_sec - 1) * 1000000000 +
			ts.tv_nsec
		);
	}
		
	return nanos;	
}

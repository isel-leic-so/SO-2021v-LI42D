#include <stdio.h>
#include <unistd.h>
#include "chrono.h"

int main() {
	
	chrono_t start = chrono_start();
	
	sleep(3);
	
	unsigned long micros = chrono_micros(start);
	
	printf("%lu\n", micros);
	
	return 0;
}
#include <stdlib.h>
#include <stdio.h>

#include "uthread.h"

void f1(void * arg) {
	char * name = (char *)arg;
	printf("%s: step 1\n", name);
	ut_yield();
	printf("%s: step 2\n", name);
	ut_yield();
	printf("%s: step 3\n", name);
}

void f2(void * arg) {
	puts("T2: hello world");
}

int main() {
	
	ut_init();
	
	ut_create(f1, "T1");
	ut_create(f2, NULL);
	ut_create(f1, "T3");
	
	puts(":: START ::");
	ut_run();
	
	puts(":: END ::");
	ut_end();
	
	return 0;
}

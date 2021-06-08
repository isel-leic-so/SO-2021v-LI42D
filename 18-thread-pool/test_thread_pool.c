#include <stdio.h>
#include <unistd.h>
#include "thread_pool.h"

#define NTHREADS 3

void test_func(void * arg) {
	const char * msg = (const char *)arg;
	puts(msg);
	usleep(1);
}

int main() {
	thread_pool_t tp;
	
	thread_pool_init(&tp, NTHREADS);
	
	thread_pool_submit(&tp, test_func, "msg01");
	thread_pool_submit(&tp, test_func, "msg02");
	thread_pool_submit(&tp, test_func, "msg03");
	thread_pool_submit(&tp, test_func, "msg04");
	thread_pool_submit(&tp, test_func, "msg05");
	thread_pool_submit(&tp, test_func, "msg06");
	thread_pool_submit(&tp, test_func, "msg07");
	thread_pool_submit(&tp, test_func, "msg08");
	
	//sleep(8);
	
	thread_pool_cleanup(&tp);
	return 0;
}

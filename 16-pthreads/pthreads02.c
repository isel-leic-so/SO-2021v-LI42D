#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 8
#define NITERATIONS 10000

int counter = 0;

void * thread_func(void * arg) {
	for (int i = 0; i < NITERATIONS; ++i) {
		++counter;
	}
	return NULL;
}

// int pthread_create(
//     pthread_t *thread,
//     const pthread_attr_t *attr,
//     void *(*start_routine) (void *),
//     void *arg
// );

int main() {
	
	pthread_t thread[NTHREADS];
	
	printf(":: START ::\n");

	for (int n = 0; n < NTHREADS; ++n) {
		pthread_create(&thread[n], NULL, thread_func, NULL);
	}
	
	printf(":: RUNNING ::\n");
	
	for (int n = 0; n < NTHREADS; ++n) {
		pthread_join(thread[n], NULL);
	}
	
	printf(":: DONE ::\n");

	printf("Expected: %d\n", NTHREADS * NITERATIONS);
	printf("Actual  : %d\n", counter);

	printf(":: END ::\n");

	return 0;
}

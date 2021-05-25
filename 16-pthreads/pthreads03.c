#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NTHREADS 8
#define NITERATIONS 200000

pthread_mutex_t cnt_lock; // mutex: mutual exclusion
int counter = 0;

void * thread_func(void * arg) {
	for (int i = 0; i < NITERATIONS; ++i) {
		pthread_mutex_lock(&cnt_lock);
		++counter;
		pthread_mutex_unlock(&cnt_lock);
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
	pthread_mutex_init(&cnt_lock, NULL);
	
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

	pthread_mutex_destroy(&cnt_lock);

	return 0;
}

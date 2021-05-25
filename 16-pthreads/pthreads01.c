#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int a = 88;

void * thread_func(void * arg) {
	const char * name = (const char *)arg;
	int b = 99;
	for (int i = 0; i < 8; ++i) {
		printf("%s : [&a : %p ; &b : %p]\n", name, &a, &b);
		sleep(1);
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
	
	pthread_t thread1, thread2;
	
	pthread_create(&thread1, NULL, thread_func, "Thread 1");
	pthread_create(&thread2, NULL, thread_func, "Thread 2");
	
	printf(":: RUNNING ::\n");
	
	pthread_join(thread2, NULL);
	pthread_join(thread1, NULL);
	
	return 0;
}

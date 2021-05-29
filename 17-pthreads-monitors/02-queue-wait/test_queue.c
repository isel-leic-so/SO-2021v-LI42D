#include <unistd.h>
#include <stdio.h>
#include "queue.h"

#define N 10000

void * source_thread(void * arg) {
	queue_t * queue = (queue_t *)arg;
	for (int i = 0; i < N; ++i) {
		int * pi = (int *)malloc(sizeof(int));
		*pi = i;
		queue_put(queue, pi);
		usleep(1000000);
	}
	return NULL;
}

int main() {

	queue_t queue;
	pthread_t source1, source2;
	
	queue_init(&queue);
	
	printf(":: START ::\n");
	
	pthread_create(&source1, NULL, source_thread, &queue);
	pthread_create(&source2, NULL, source_thread, &queue);
	
	for (int i = 0; i < N*2; ) {
		int * pi = (int *)queue_get(&queue);
		if (pi) {
			free(pi);
			++i;
			putchar('.');
			fflush(stdout);
		} else {
			putchar('_');
		}
	}
	
	printf("\n:: DONE ::\n");
	
	pthread_join(source1, NULL);
	pthread_join(source2, NULL);
	
	printf(":: END ::\n");

	queue_cleanup(&queue);
	return 0;
}

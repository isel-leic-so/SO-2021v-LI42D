#include "countdownlatch.h"

void countdownlatch_init(countdownlatch_t * cdl, int initial_count) {
	pthread_mutex_init(&cdl->lock, NULL);
	pthread_cond_init(&cdl->waiters, NULL);
	cdl->counter = initial_count;
}

void countdownlatch_cleanup(countdownlatch_t * cdl) {
	pthread_mutex_destroy(&cdl->lock);
	pthread_cond_destroy(&cdl->waiters);
}

void countdownlatch_countdown(countdownlatch_t * cdl) {
	pthread_mutex_lock(&cdl->lock);
	{
		if (cdl->counter > 0) {
			cdl->counter -= 1;
			if (cdl->counter == 0) {
				pthread_cond_broadcast(&cdl->waiters);
			}
		}
	}
	pthread_mutex_unlock(&cdl->lock);
}

void countdownlatch_await(countdownlatch_t * cdl) {
	pthread_mutex_lock(&cdl->lock);
	{
		while (cdl->counter > 0) {
			pthread_cond_wait(&cdl->waiters, &cdl->lock);
		}
	}
	pthread_mutex_unlock(&cdl->lock);
}

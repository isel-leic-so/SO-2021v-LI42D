#pragma once

#include <pthread.h>

typedef struct countdownlatch {
	pthread_mutex_t lock;
	pthread_cond_t waiters;
	int counter;
} countdownlatch_t;

void countdownlatch_init(countdownlatch_t * cdl, int initial_count);
void countdownlatch_cleanup(countdownlatch_t * cdl);

void countdownlatch_countdown(countdownlatch_t * cdl); // desbloqueante
void countdownlatch_await(countdownlatch_t * cdl);     // bloqueante

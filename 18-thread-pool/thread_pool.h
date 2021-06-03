#pragma once

#include <pthread.h>
#include <stdlib.h>
#include "queue.h"

typedef void (*thread_pool_action_t)(void * arg);

typedef struct thread_pool {
	size_t     nthreads;
	pthread_t * threads;
	queue_t  work_queue;
} thread_pool_t;

void thread_pool_init(thread_pool_t * tp, size_t nthreads);

void thread_pool_cleanup(thread_pool_t * tp);

void thread_pool_submit(thread_pool_t * tp, thread_pool_action_t action, void * arg);

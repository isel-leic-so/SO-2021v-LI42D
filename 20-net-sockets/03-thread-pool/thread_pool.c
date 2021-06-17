#include "thread_pool.h"

typedef struct workitem {
	thread_pool_action_t action;
	void * arg;
} workitem_t;

static void DEATH_PILL(void * arg) {}

static void * thread_pool_worker(void * arg) {
	thread_pool_t * tp = (thread_pool_t *)arg;
	for (;;) {
		workitem_t * wi = (workitem_t *)queue_get(&tp->work_queue);
		
		thread_pool_action_t action = wi->action;
		void * arg = wi->arg;
		
		free(wi);
		
		if (action == DEATH_PILL) {
			break;
		}

		action(arg);
	}
	return NULL;
}

void thread_pool_init(thread_pool_t * tp, size_t nthreads) {
	queue_init(&tp->work_queue);
	tp->stopped = false;
	tp->threads = (pthread_t *)malloc(nthreads * sizeof (pthread_t));
	tp->nthreads = nthreads;
	for (size_t n = 0; n < nthreads; ++n) {
		pthread_create(&tp->threads[n], NULL, thread_pool_worker, tp);
	}
}

void thread_pool_cleanup(thread_pool_t * tp) {
	thread_pool_stop(tp);
	
	for (size_t n = 0; n < tp->nthreads; ++n) {
		pthread_join(tp->threads[n], NULL);
	}
	free(tp->threads);
	queue_cleanup(&tp->work_queue);
}

static void thread_pool_internal_submit(thread_pool_t * tp, thread_pool_action_t action, void * arg) {
	workitem_t * wi = (workitem_t *)malloc(sizeof (workitem_t));
	wi->action = action;
	wi->arg = arg;
	queue_put(&tp->work_queue, wi);
}

bool thread_pool_submit(thread_pool_t * tp, thread_pool_action_t action, void * arg) {
	if (!tp->stopped) {
		thread_pool_internal_submit(tp, action, arg);
		return true;
	}
	return false;
}

void thread_pool_stop(thread_pool_t * tp) {
	if (!tp->stopped) {
		tp->stopped = true;

		for (int i = 0; i < tp->nthreads; ++i) {
			thread_pool_internal_submit(tp, DEATH_PILL, NULL);
		}
	}
}

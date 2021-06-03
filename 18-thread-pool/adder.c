#include <stdio.h>

#include <limits.h>
#include <assert.h>
#include "chrono.h"

#include "countdownlatch.h"
#include "thread_pool.h"

#define NITERS 50
#define NVALS 5000000
#define NTHREADS 4
 
thread_pool_t tpool; 
 
int vals[NVALS];

static void init_vals() {
	for(int i = 0; i < NVALS; ++i)
		vals[i] = 1;
}

long seq_adder(int vals[], size_t nvals) {
	long total = 0;
	for (size_t i = 0; i < nvals; ++i)
		total += vals[i];
	return total;
}

typedef struct sum_task {
	countdownlatch_t * cdl;
	int * begin;
	int * end;
	long total;
} sum_task_t;

void par_func(void * arg) {
	sum_task_t * stask = (sum_task_t *)arg;
	long total = 0;
	for (int * p = stask->begin; p < stask->end; ++p) {
		total += *p;
	}
	stask->total = total;
	countdownlatch_countdown(stask->cdl);
}

long par_adder(int vals[], size_t nvals) {
	
	countdownlatch_t cdl;
	sum_task_t tasks[NTHREADS];
	size_t region_size = (nvals + (NTHREADS - 1)) / NTHREADS;

	countdownlatch_init(&cdl, NTHREADS);
	for (size_t n = 0; n < NTHREADS; ++n) {
		tasks[n].cdl = &cdl;
		tasks[n].begin = &vals[n * region_size];
		tasks[n].end = (tasks[n].begin + region_size) > &vals[nvals] ?
			&vals[nvals] :
			(tasks[n].begin + region_size);
		thread_pool_submit(&tpool, par_func, &tasks[n]);
	}

	// quando acabarem...
	countdownlatch_await(&cdl);

	long total = 0;
	for (int i = 0; i < NTHREADS; ++i) {
		total += tasks[i].total;
	}
	return total;
}

void test(char *name,
		long (*sum_func)(int vals[], size_t nvals), 
		int vals[], 
		int nvals, 
		long expected) {
	
	unsigned long micros, less_micros = ULONG_MAX;
	long res;
	for(int i = 0; i < NITERS; ++i) {
		chrono_t chrono = chrono_start();
		{
			res = sum_func(vals, nvals);
		}
		micros = chrono_micros(chrono);

		if (micros < less_micros) 
			less_micros = micros;

		assert(expected == res);
	}
	
	printf("%s gives a result of %ld in %lu micros.\n", 
			name, res, less_micros);
}

int main() {
	init_vals();
	 
	thread_pool_init(&tpool, NTHREADS); 
	 
	test("sequential adder", seq_adder, vals, NVALS, NVALS);
	test("parallel adder", par_adder, vals, NVALS, NVALS);
	
	//thread_pool_cleanup(&tpool);
	return 0;
}

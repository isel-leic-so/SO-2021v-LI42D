#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/mman.h>

#include "list.h"
#include "uthread.h"

#define STACK_SIZE (8*4096)

struct uthread {
	uint64_t rsp;
	void (*start_routine)(void *);
	void * arg;
	list_entry_t entry;
};

typedef struct uthread_context {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbx;
	uint64_t rbp;
	void (*ret_addr)();
} uthread_context_t;

uthread_t * running_thread;
uthread_t * main_thread;

list_entry_t ready_queue;

void context_switch(uthread_t * currThread, uthread_t * nextThread);

void internal_exit(uthread_t * currThread, uthread_t * nextThread);

void internal_start() {
	running_thread->start_routine(running_thread->arg);
	ut_exit();
}

void cleanup_thread(uthread_t * puthread) {
	munmap(puthread, STACK_SIZE);
}

uthread_t * extract_next_ready_thread() {
	return is_list_empty(&ready_queue) ?
		main_thread :
		container_of(remove_head_list(&ready_queue), uthread_t, entry);
}

void schedule() {
	uthread_t * next_thread = extract_next_ready_thread();
	context_switch(running_thread, next_thread);
}

void ut_init() {
	init_list_head(&ready_queue);
}

void ut_end() {
	if (!is_list_empty(&ready_queue)) {
		fprintf(stderr, "ERROR: ready queue not empty at exit\n");
	}
}

void ut_run() {
	uthread_t the_main_thread;
	
	running_thread = main_thread = &the_main_thread;
	
	schedule();
}

uthread_t * ut_create(void (*start_routine)(void *), void * arg) {
	uthread_t * puthread = (uthread_t *)mmap(NULL, STACK_SIZE, PROT_READ|PROT_WRITE, MAP_ANONYMOUS|MAP_PRIVATE, -1, 0);
	
	uthread_context_t * pctx =
		(uthread_context_t *)(((uint8_t *)puthread + STACK_SIZE) - sizeof (uthread_context_t));
	pctx->rbp = 0;
	pctx->ret_addr = internal_start;
	
	puthread->rsp = (uint64_t)pctx;
	puthread->start_routine = start_routine;
	puthread->arg = arg;
	
	insert_tail_list(&ready_queue, &(puthread->entry));
	
	return puthread;
}

void ut_yield() {
	if (!is_list_empty(&ready_queue)) {
		insert_tail_list(&ready_queue, &(running_thread->entry));
		schedule();
	}
}

void ut_exit() {
	internal_exit(running_thread, extract_next_ready_thread());
}

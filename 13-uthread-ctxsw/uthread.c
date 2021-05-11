#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define STACK_SIZE (8*4096)

typedef struct uthread_context {
	uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t rbx;
	uint64_t rbp;
	void (*ret_addr)();
} uthread_context_t;

typedef struct uthread {
	uint64_t rsp;
	uint8_t stack[STACK_SIZE];
} uthread_t;

void context_switch(uthread_t * currThread, uthread_t * nextThread);

void prepare_thread(uthread_t * thread, void (*func)()) {
	uthread_context_t * pctx =
		(uthread_context_t *)((thread->stack + STACK_SIZE) - sizeof (uthread_context_t));
	pctx->rbp = 0;
	pctx->ret_addr = func;
	thread->rsp = (uint64_t)pctx;
}

uthread_t mainThread;

uthread_t thread1;
uthread_t thread2;
uthread_t thread3;

void f1() {
	puts("T1: step 1");
	context_switch(&thread1, &thread2);
	puts("T1: step 2");
	context_switch(&thread1, &thread3);
	puts("T1: step 3");
	context_switch(&thread1, &thread3);
}

void f2() {
	puts("T2: hello world");
	context_switch(&thread2, &thread3);
}

void f3() {
	puts("T3: step 1");
	context_switch(&thread3, &thread1);
	puts("T3: step 2");
	context_switch(&thread3, &thread1);
	puts("T3: step 3");
	puts(":: END ::");
	exit(0);
}

int main() {
	prepare_thread(&thread1, f1);
	prepare_thread(&thread2, f2);
	prepare_thread(&thread3, f3);
	
	puts(":: START ::");
	context_switch(&mainThread, &thread1);
	
	puts("xx ERROR xx");
	return 1;
}

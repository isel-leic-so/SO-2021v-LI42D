#ifndef UTHREAD_H
#define UTHREAD_H

#include <stdint.h>

typedef struct uthread uthread_t;

void ut_init();
void ut_run();
void ut_end();

uthread_t * ut_create(void (*start_routine)(void *), void * arg);
void ut_exit();

void ut_yield();

#endif

/////////////////////////////////////////////////////////////////
//
// CCISEL 
// 2007-2021
//
// UThread library:
//   User threads supporting cooperative multithreading.
//
// Authors:
//   Carlos Martins, João Trindade, Duarte Nunes, Jorge Martins
// 

#pragma once

#include "uthread.h"
#include "list.h"

//
// A single-shot broadcast event. Threads are kept blocked in wait
// until set is called.
//
 
typedef struct event {
	bool signaled;
	list_entry_t waiters;
} event_t;
 
void event_init(event_t * event);
 
void event_wait(event_t * event);
 
void event_set(event_t * event);

INLINE bool event_value(event_t * event) {
	return event->signaled; 
}


//
// A semaphore, containing the current number of units, upper bounded
// by limit.
//
typedef struct semaphore {
	list_entry_t waiters;
	int units;
	int limit;
} semaphore_t;

//
// Initializes a semaphore instance. Units is the starting number of
// available permits and limit is the maximum number of units allowed
// for the specified semaphore instance.
//
void sem_init(semaphore_t * sem, int units, int limit);

//
// Gets the specified number of units from the semaphore. If there
// aren't enough units available, the calling thread is blocked until
// they are added by a call to sem_post.
//
void sem_wait(semaphore_t * sem, int units);

//
// Adds the specified number of units to the semaphore, possibly
// unblocking waiting threads.
//
void sem_post(semaphore_t * sem, int units);

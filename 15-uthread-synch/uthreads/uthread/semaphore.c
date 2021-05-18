#include "usynch.h"
#include "waitblock.h"

//
// Wait block used to queue requests on semaphores.
//
typedef struct semaphore_waitblock {
	waitblock_t header;
	int units;
} semaphore_waitblock_t;


//
// Initializes the specified sem wait block.
//
static void init_sem_waitblock(semaphore_waitblock_t * sem_wblock, int units) {
	init_waitblock(&sem_wblock->header);
	sem_wblock->units = units;
}


//
// Initializes a sem instance. units is the starting number of available units and 
// limit is the maximum number of units allowed for the specified sem instance.
//
void sem_init(semaphore_t * sem, int units, int limit) {
	init_list_head(&sem->waiters);
	sem->units = units;
	sem->limit = limit;
}

//
// Gets the specified number of units from the sem. If there aren't enough units available,  
// the calling thread is blocked until they are added by a call to sem_post().
//
void sem_wait(semaphore_t * sem, int units) {
	semaphore_waitblock_t waitblock;

	//
	// If there are enough units available, get them and keep running.
	//
	if (is_list_empty(&sem->waiters) && sem->units >= units) {
		sem->units -= units;
		return;
	}

	//
	// There are no units available. Insert the running thread in the wait list.
	//
	init_sem_waitblock(&waitblock, units);   
	insert_list_last(&sem->waiters, &waitblock.header.entry);

	//
	// Remove the current thread from the ready list.
	//
	ut_deactivate();
}

//
// Adds the specified number of units to the sem, eventually unblocking waiting threads.
//
void sem_post(semaphore_t * sem, int units) {
	list_entry_t * entry;
	list_entry_t * waiters;
	semaphore_waitblock_t * waitblock;

	if ((sem->units += units) > sem->limit) {
		sem->units = sem->limit;
	}

	waiters = &sem->waiters;

	//
	// Release all blocked thread whose request can be satisfied.
	//
	while (sem->units > 0 && (entry = waiters->next) != waiters) {
		waitblock = container_of(entry, semaphore_waitblock_t, header.entry);

		if (sem->units < waitblock->units) {
			//
			// We stop at the first request that cannot be satisfied to ensure FIFO ordering.
			//
			break;
		}

		sem->units -= waitblock->units;
		remove_list_first(waiters);
		ut_activate(waitblock->header.thread);
	}
}

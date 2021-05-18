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

#include "uthread.h"
#include "usynch.h"
#include "waitblock.h"

void event_init(event_t * event) {
	event->signaled = false;
	init_list_head(&event->waiters);
}

void event_wait (event_t * event) {
	if (event->signaled == false) {
		waitblock_t wblock;
		init_waitblock(&wblock);
		insert_list_last(&event->waiters, &wblock.entry);
		ut_deactivate();
	}
}

void event_set(event_t * event) {
	event->signaled = true;
	while (!is_list_empty(&(event->waiters))) {
		list_entry_t * curr = remove_list_first(&(event->waiters));
		waitblock_t * pwblock = container_of(curr, waitblock_t, entry);
		ut_activate(pwblock->thread);
	}
}

#include "usynch.h"

void event_init(event_t * event) {
	event->signaled = false;
	init_list_head(&event->waiters);
}

void event_wait(event_t * event) {
	if (!event->signaled) {
		waitblock_t wblock;
		init_waitblock(&wblock);
		insert_list_last(&event->waiters, &wblock->entry);
		ut_deactivate();
	}
}

void event_set(event_t * event) {
	event->signaled = true;
	while (!is_list_empty(&event->waiters)) {
		list_entry_t * curr = remove_list_first(&event->waiters);
		waitblock_t * pblock = container_of(curr, waitblock_t, entry);
		ut_activate(pblock->thread);
	}
}

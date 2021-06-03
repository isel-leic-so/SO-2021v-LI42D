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

#ifndef LIST_H
#define LIST_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// The attribute always_inline forces inlining even when compiling
// without optimizations.
#define INLINE __attribute__((always_inline)) static inline

typedef struct list_entry  {
	struct list_entry *next;
	struct list_entry *prev;
} list_entry_t;

#ifndef container_of
#define container_of(address, type, field) \
	((type *)((uint8_t *)(address) - (size_t)(&((type *)0)->field)))
#endif

// Initializes the specified doubly linked list.
//
INLINE void init_list_head(list_entry_t * listhead) {
	listhead->next = listhead->prev = listhead;
}

// Returns true if the specified list is empty.
//
INLINE bool is_list_empty(list_entry_t * listhead) {
	return listhead->next == listhead;
}

// Removes the specified entry from the list that
// contains it.
//
INLINE bool remove_list_entry(list_entry_t * entry) {
	list_entry_t * prev;
	list_entry_t * next;

	next = entry->next;
	prev = entry->prev;
	prev->next = next;
	next->prev = prev;

	return next == prev;
}

// Removes the entry that is at the front of the list.
//
INLINE list_entry_t * remove_list_first(list_entry_t * listhead) {
	list_entry_t * entry = listhead->next;
	remove_list_entry(entry);
	return entry;
}

// Removes the entry that is at the tail of the list.
//
INLINE list_entry_t * remove_list_last(list_entry_t * listhead) {
	list_entry_t * entry = listhead->prev;
	remove_list_entry(entry);
	return entry;
}

// Inserts the specified new entry after the specified reference entry.
//
INLINE void insert_list_after(list_entry_t * ref_entry, list_entry_t * new_entry) {
	list_entry_t * prev = ref_entry;
	list_entry_t * next = ref_entry->next;

	new_entry->prev = prev;
	new_entry->next = next;
	prev->next = new_entry;
	next->prev = new_entry;
}

// Inserts the specified entry at the tail of the list.
//
INLINE void insert_list_last(list_entry_t * listhead, list_entry_t * entry) {
	insert_list_after(listhead->prev, entry);
}

// Inserts the specified entry at the head of the list.
//
// The attribute always_inline force inline even without compiling with optimizations
INLINE void insert_list_first(list_entry_t * listhead, list_entry_t * entry) {
	insert_list_after(listhead, entry);
}

#endif

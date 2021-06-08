#include <string.h>
#include <stddef.h>

#include "memchannel.h"
#include "shmem.h"

#define SEM_SHARED 1

// #define offsetof(TYPE, FIELD) ((size_t)(&(((TYPE *)0)->FIELD)))

memch_t * mc_create(size_t elem_size, size_t capacity, const char * name) {
	size_t items_size = elem_size * capacity;
	size_t total_size = offsetof(memch_t, items) + items_size;
	
	memch_t * mc = (memch_t *)shmem_create(name, total_size);
	if (!mc) {
		return NULL;
	}

	mc->elem_size = elem_size;
	mc->capacity = capacity;

	mc->total_size = total_size;

	strncpy(mc->name, name, MEMCH_MAX_NAME_LEN);
	mc->name[MEMCH_MAX_NAME_LEN - 1] = 0;

	mc->iput = mc->iget = 0;
	
	pthread_mutexattr_t lock_attr;
	pthread_mutexattr_init(&lock_attr);
	pthread_mutexattr_setpshared(&lock_attr, PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(&(mc->lock), &lock_attr);
	
	sem_init(&(mc->has_items), SEM_SHARED, 0);
	sem_init(&(mc->has_space), SEM_SHARED, capacity);
	
	return mc;
}

memch_t * mc_open(const char * name) {
	memch_t * mc = (memch_t *)shmem_open(name);
	return mc;
}

void mc_close(memch_t * mc) {
	shmem_close(mc, mc->total_size);
}

void mc_destroy(memch_t * mc) {
	char name[MEMCH_MAX_NAME_LEN];
	strcpy(name, mc->name);
	
	sem_destroy(&(mc->has_items));
	sem_destroy(&(mc->has_space));

	pthread_mutex_destroy(&(mc->lock));

	shmem_destroy(name, mc, mc->total_size);
}
	
void mc_put(memch_t * mc, void * item) {
	sem_wait(&(mc->has_space));
	pthread_mutex_lock(&(mc->lock));
	{
		size_t iput = mc->iput++ % mc->capacity;
		memcpy(mc->items + iput * mc->elem_size, item, mc->elem_size);
	}
	pthread_mutex_unlock(&(mc->lock));
	sem_post(&(mc->has_items));
}

void mc_get(memch_t * mc, void * item) {
	sem_wait(&(mc->has_items));
	pthread_mutex_lock(&(mc->lock));
	{
		size_t iget = mc->iget++ % mc->capacity;
		memcpy(item, mc->items + iget * mc->elem_size, mc->elem_size);
	}
	pthread_mutex_unlock(&(mc->lock));
	sem_post(&(mc->has_space));
}

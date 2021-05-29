#pragma once

#include <stdlib.h>
#include <pthread.h>
#include "list.h"

typedef struct queue {
	pthread_mutex_t lock;   // sincronização de dados
	list_entry_t head;
	size_t size;
} queue_t;

void   queue_init(queue_t * queue);
void   queue_cleanup(queue_t * queue);

void   queue_put(queue_t * queue, void * item);
void * queue_get(queue_t * queue);

size_t queue_size(queue_t * queue);

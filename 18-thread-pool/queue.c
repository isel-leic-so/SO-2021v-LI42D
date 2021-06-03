#include "queue.h"

typedef struct queue_node {
	void * item;
	list_entry_t entry;
} queue_node_t;

void queue_init(queue_t * queue) {
	pthread_mutex_init(&queue->lock, NULL);
	pthread_cond_init(&queue->waiters, NULL);
	init_list_head(&queue->head);
	queue->size = 0;
}

void queue_cleanup(queue_t * queue) {
	pthread_mutex_destroy(&queue->lock);
	pthread_cond_destroy(&queue->waiters);
}

size_t queue_size(queue_t * queue) {
	size_t size;
	pthread_mutex_lock(&queue->lock);
	{
		size = queue->size;
	}
	pthread_mutex_unlock(&queue->lock);
	return size;
}

void queue_put(queue_t * queue, void * item) {
	queue_node_t * pnode = (queue_node_t *)malloc(sizeof(queue_node_t));
	pnode->item = item;
	pthread_mutex_lock(&queue->lock);
	{ 
		insert_list_last(&queue->head, &pnode->entry);
		queue->size++;

		pthread_cond_signal(&queue->waiters); // liberta uma das threads bloqueadas em wait
	}
	pthread_mutex_unlock(&queue->lock);
}

void * queue_get(queue_t * queue) {
	list_entry_t * pentry = NULL;

	pthread_mutex_lock(&queue->lock);
	{
		while (queue->size == 0) {
			pthread_cond_wait(&queue->waiters, &queue->lock);  // bloqueia até signal (ou broadcast)
		}

		pentry = remove_list_first(&queue->head);
		queue->size--;
	}
	pthread_mutex_unlock(&queue->lock);
		
	queue_node_t * pnode = container_of(pentry, queue_node_t, entry);
	void * item = pnode->item;

	free(pnode);
	
	return item;
}

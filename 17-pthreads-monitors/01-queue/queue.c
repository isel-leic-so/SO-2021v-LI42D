#include "queue.h"

typedef struct queue_node {
	void * item;
	list_entry_t entry;
} queue_node_t;

void queue_init(queue_t * queue) {
	pthread_mutex_init(&queue->lock, NULL);
	init_list_head(&queue->head);
	queue->size = 0;
}

void queue_cleanup(queue_t * queue) {
	pthread_mutex_destroy(&queue->lock);
}

size_t queue_size(queue_t * queue) {
	return queue->size;
}

void queue_put(queue_t * queue, void * item) {
	queue_node_t * pnode = (queue_node_t *)malloc(sizeof(queue_node_t));
	pnode->item = item;
	pthread_mutex_lock(&queue->lock);
	{
		insert_list_last(&queue->head, &pnode->entry);
		queue->size++;
	}
	pthread_mutex_unlock(&queue->lock);
}

void * queue_get(queue_t * queue) {
	void * item = NULL;
	pthread_mutex_lock(&queue->lock);
	{
		if (!is_list_empty(&queue->head)) {
			list_entry_t * pentry = remove_list_first(&queue->head);
			queue->size--;
			
			queue_node_t * pnode = container_of(pentry, queue_node_t, entry);
			item = pnode->item;
			
			free(pnode);
		}
	}
	pthread_mutex_unlock(&queue->lock);
	return item;
}

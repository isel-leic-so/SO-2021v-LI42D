#pragma once 

#include <stdlib.h>


////////////////////
//
// API for servers
//

void * shmem_create(const char *name, size_t size);

void shmem_destroy(const char * name, void *base, size_t size);


////////////////////
//
// API for clients
//

void * shmem_open(const char *name);

void shmem_close(void *base, size_t size);


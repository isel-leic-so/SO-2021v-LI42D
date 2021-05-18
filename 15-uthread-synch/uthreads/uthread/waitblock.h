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
// Wait block used to queue requests on synchronizers.
//

typedef struct waitblock {
	list_entry_t entry;
	uthread_t * thread;
} waitblock_t;

//
// Initializes the specified wait block.
//
INLINE void init_waitblock(waitblock_t * waitblock) {
	waitblock->thread = ut_self();
}

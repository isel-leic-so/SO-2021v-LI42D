// simpletest.c : Defines the entry point for the console application.
//

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#include "uthread.h"


/////////////////////////////////////////////
//
// CCISEL 
// 2007-2019
//
// UThread Library First Test
//
// Jorge Martins, 2019
//
////////////////////////////////////////////

#define DEBUG

#define MAX_THREADS 10


/////////////////////////////////////////////////////////////
//                                                         //
// Test 1: N threads, each one printing its number M times //
//                                                         //
/////////////////////////////////////////////////////////////

uint64_t test1_count;

void test1_thread (void * argument) {
	uint8_t c;
	uint64_t index;
	c = (uint8_t) (uint64_t) argument;	
	 
	for (index = 0; index < 100; ++index) {
	    putchar(c); fflush(stdout);
		 
	    if ((rand() % 4) == 0) {
			usleep(100000); 
		    ut_yield();
	    }	 
    }
	++test1_count;
}

void test1 ()  {
	uint64_t index;

	test1_count = 0; 

	printf("\n :: Test 1 - BEGIN :: \n\n");

	for (index = 0; index < MAX_THREADS; ++index) {
		ut_create(test1_thread, (void *) ('0' + index));
	}   

	ut_run();

	assert(test1_count == MAX_THREADS);

	printf("\n\n :: Test 1 - END :: \n");
}


// Test to run in single step

void func1(void * arg) {

	printf("Start func1\n");
	
	ut_yield();
 
	printf("End func1\n");

}

void func2(void * arg) {
	printf("func2\n");
}

void test2() {
	printf("\n :: Test 2 - BEGIN :: \n\n");
	
	ut_create(func1, NULL);
	ut_create(func2, NULL);
	ut_run();

	printf("\n\n :: Test 2 - END :: \n");
}



int main () {
	ut_init();
 
	test1();	
	 
	ut_end();
	return 0;
}



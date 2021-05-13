all: uthreads lists

uthreads: ctxsw.s uthread.c test_uthread.c
	gcc -Wall -pedantic -g -o uthreads test_uthread.c uthread.c ctxsw.s

lists: test_list.c list.h
	gcc -Wall -pedantic -g -o lists test_list.c

clean:
	rm -f uthreads lists

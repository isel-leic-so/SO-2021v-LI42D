
dynamic: simpletest.c
	gcc -o simpletest -Wall -g -I ../include simpletest.c ../uthread/libuthread.so
	
	
static : simpletest.c
	gcc -o simpletest -Wall -g -I ../include simpletest.c ../uthread/libuthread.a
	
clean:
	rm -rf simpletest

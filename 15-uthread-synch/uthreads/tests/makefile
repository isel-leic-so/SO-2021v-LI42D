
dynamic: simpletest.c event_test.c
	gcc -o simpletest -Wall -g -I ../include simpletest.c ../uthread/libuthread.so
	gcc -o event_test -Wall -g -I ../include event_test.c ../uthread/libuthread.so
	
	
static : simpletest.c event_test.c
	gcc -o simpletest -Wall -g -I ../include simpletest.c ../uthread/libuthread.a
	gcc -o event_test -Wall -g -I ../include event_test.c ../uthread/libuthread.a
	
clean:
	rm -rf simpletest event_test

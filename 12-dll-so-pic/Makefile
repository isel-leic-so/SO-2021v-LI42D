all: pic nopic test-libex

pic: libex-pic.s libex-pic.so

nopic: libex-nopic.s libex-nopic.so

libex-pic.s: libex.c
	gcc -mcmodel=large -S -fPIC -o libex-pic.s libex.c

libex-nopic.s: libex.c
	gcc -mcmodel=large -S -fno-pic -o libex-nopic.s libex.c

libex-pic.so: libex.c
	gcc -mcmodel=large -shared -fPIC -o libex-pic.so libex.c

libex-nopic.so: libex.c
	gcc -mcmodel=large -shared -fno-pic -o libex-nopic.so libex.c

test-libex: test-libex.c
	gcc -o test-libex test-libex.c -ldl

clean:
	rm -f libex-*.s libex-*.so test-libex

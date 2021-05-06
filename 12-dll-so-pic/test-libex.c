#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

char libex_name[256];

int main(int argc, char * argv[]) {
	sprintf(libex_name, "./libex-%s.so", argc > 1 ? argv[1] : "pic");
	
	printf("PID: %u\n", getpid());
	
	printf("#1"); getchar();

	void * libex = dlopen(libex_name, RTLD_LOCAL | RTLD_NOW);
	printf("libex loaded at %p\n", libex);
	
	printf("#2"); getchar();

	dlclose(libex);
	
	return 0;
}

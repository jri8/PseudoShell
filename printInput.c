#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(){

	pid_t pid = getpid();
	printf("printInput has process id: %d\n", pid);
	printf("the following is read from stdin by printInput\n");
	printf("------------------------------------------------\n");
	int c;
	FILE* stream = fdopen(0, "r");
	while ((c = fgetc (stream)) != EOF)
		putchar (c);
	fclose (stream);
	
	return 0;
}

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
int main(){

	pid_t pid = getpid();
	printf("printMidde has process id: %d\n",pid);
	int c;
	FILE* stream = fdopen(0, "r");
	while ((c = fgetc (stream)) != EOF)
		putchar (c);
	fclose (stream);
	
	return 0;
}

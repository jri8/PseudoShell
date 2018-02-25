#include <stdio.h>
#include <stdlib.h>
int main(int argc, char* argv[]){	
	int i = 0;
	int count;
	if (argc == 1)
		count = 1;
	else
		count = atoi(argv[1]);
    	for(i; i < count; i++)
        	printf("hello world\n");
}

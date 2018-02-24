#include <stdlib.h>
#include <stdio.h>

int main(){
	char buf[2048];
	fgets(buf, sizeof buf, stdin);
	printf("%s\n", buf);	
	
//	int c;
//	FILE* stream = fdopen(0, "r");
//	while ((c = fgetc (stream)) != EOF)
//		putchar (c);
//	fclose (stream);
	
	return 0;
}

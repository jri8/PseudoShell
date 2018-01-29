//main file for Pseudo Shell
#include <stdio.h>
#include <string.h>
int main(void){
	
	char buffer[2048];
	memset(buffer,0,sizeof(buffer));

	do{
		printf("$");
		fgets(buffer,2048, stdin);
		printf("%s", buffer);
		int x = strncmp(buffer, "exit", 4);
		printf("%d\n", x);
	}while(strncmp(buffer, "exit", 4) != 0);

	return 0;
	
}



// Testing commit (Greg)

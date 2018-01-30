//main file for Pseudo Shell
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*----------TO-DO-----------
//tokenize input
//figureout wtf to do
//fork()
//manage stdin/out/err
//execve
//make other files with code to execute given commands
//(exit, cd, history)
//error messages
------------TO-DO----------*/






int main(void){
	
	char buffer[2048];
	memset(buffer,0,sizeof(buffer));
	char *history[100];
	int histInd = 0;

	char *token;
	//use circular array to keep track of history
	do{
		//displaying prompt
		printf("$");
		
		//reading input
		fgets(buffer,2048, stdin);

		//allocating space in array for history
		free(history[histInd]); 
		history[histInd] = malloc(strlen(buffer)+1);
		//copies buffer into history array
		strcpy(history[histInd], buffer);
		//maintaining index
		histInd++;
		histInd = histInd % 100;
		
		//tokenizing input
		token = strtok(buffer, " ");	
		while (token != NULL){
			printf("%s\n", token);
			token = strtok(NULL, " ");
		}
		
	}while(strncmp(buffer, "exit", 4) != 0);
	int i;
	for(i = 0; i < histInd; i++){
		printf("%d %s",i, history[i]);
	}
	
	return 0;
	
}



// Testing commit (Greg)

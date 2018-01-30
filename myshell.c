//main file for Pseudo Shell
#include <stdio.h>
#include <string.h>
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
	//use circular array to keep track of history
	do{
		printf("$");
		fgets(buffer,2048, stdin);
		history[histInd] = malloc(strlen(buffer)+1);
		strcpy(history[histInd], buffer);

	//	history[histInd] = buffer;
		histInd++;
		histInd = histInd % 100;
		
	}while(strncmp(buffer, "exit", 4) != 0);
	int i;
	for(i = 0; i < histInd; i++){
		printf("%d %s",i, history[i]);
	}
	return 0;
	
}



// Testing commit (Greg)

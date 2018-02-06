//main file for Pseudo Shell
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
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

const int MAX_HISTORY = 100;

void historyController(char *history[], int *haddr, char* token, bool *waddr){
	int histInd = (*haddr);
	int cmdInd;
	bool hasWrapped = (*waddr);
	token = strtok(NULL, " ");

	if (token != NULL){
		printf("token is not null\n");
		
		//clears history	
		if (strncmp(token, "-c", 1) == 0){
			printf("clearing history\n");
			
			memset(history, 0, sizeof(history));
			*haddr = 0;
			*waddr = false;
		}	
		//executes command at target index from history
		else if (isdigit(token[0])){
			cmdInd = atoi(token);
			printf("executing history cmd: %d\n", cmdInd);
		}
		//not a valid modifier
		else{
			printf("not a valid modifier\n");
		}		
	}
	//if not extra arguments print history		
	else{
		printf("token is null\n");
		int i;
		if (hasWrapped == false){
			for(i = 0; i < histInd; i++){
				printf("%d %s",i, history[i]);
			}
		}
		else{
			int j;
			for(i = 0; i < MAX_HISTORY; i++){
				j = (histInd + i) % MAX_HISTORY;
				printf("%d %s", i, history[j]);
			}	
		}
	}
	printf("leaving function\n");	
}

int main(void){
	
	//input buffer		
	char buffer[2048];
	memset(buffer,0,sizeof(buffer));
	
	//history
	char *history[MAX_HISTORY];
	memset(history, 0, sizeof(history));
	int histInd = 0;
	bool hasWrapped = false;

	//tokenizer
	char *token;

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
		if (histInd >= MAX_HISTORY){
			//checks if circular arr wrapped back
			hasWrapped = true;
		}
		histInd = histInd % MAX_HISTORY;
		
		//tokenizing input
		token = strtok(buffer, " ");
		if (strncmp(token, "history", 6)==0){
			printf("inside history if block\n");
			historyController(history, &histInd, token, &hasWrapped);		
		}
	
	}while(strncmp(buffer, "exit", 4) != 0);

	return 0;
	
}


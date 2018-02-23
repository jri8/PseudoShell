//main file for Pseudo Shell
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
/*----------TO-DO-----------
//finish execute program function so it works when passed in one program
//implement functionality for history [offset] (should just call parse input)
//manage stdin/out/err
//add functionality for redirect
//add functionality for pipes
//add functionality for & and ;
//error messages
------------TO-DO----------*/

void changeDirectory(char*);

const int MAX_HISTORY = 100;

void historyController(char *history[], int *haddr, char* token, bool *waddr){
	int histInd = (*haddr);
	int cmdInd;
	bool hasWrapped = (*waddr);
	token = strtok(NULL, " ");

	if (token != NULL){
//		printf("token is not null\n");
		
		//clears history	
		if (strncmp(token, "-c", 1) == 0){
//			printf("clearing history\n");
			
			memset(history, 0, sizeof(history));
			*haddr = 0;
			*waddr = false;
		}	
		//executes command at target index from history
		else if (isdigit(token[0])){
			cmdInd = atoi(token);
//			printf("executing history cmd: %d\n", cmdInd);
		}
		//not a valid modifier
		else{
			printf("not a valid modifier\n");
		}		
	}
	//if not extra arguments print history		
	else{
//		printf("token is null\n");
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
//	printf("leaving function\n");	
}

void executeProg(char* token){

	//if the input is a file to execute then call this function
	//need to pass in the name of the program, any extra tokens

	//first thing to do is to check for "|", ";", "<", ">" "&" in tokens
	//next fork one or multiple sub processes
	//based upon extra parameters provide different functionality
	
	//basic framework of fork() from HW2, simpleProccess.c
	char *ex = token;
	printf("in execProg\n");
	printf("%s\n", token);
	
	//other = strtok(NULL, " ");
	token = strtok(NULL, " ");
	while (token != NULL){
		if(strncmp(token, "|", 1) == 0){
			printf("pipe\n");
		} 
		else if(strncmp(token, ";", 1) == 0){
			printf("run second program\n");
		}
		else if (strncmp(token, "<", 1) == 0){
			printf("redirect in?\n");
		}
		else if (strncmp(token, ">", 1) == 0){
			printf("redirect out\n");
		}
		else if (strncmp(token, "&", 1) == 0){
			printf("send process to background\n");
		}
		else{
			printf("%s\n", token);
		}
		token = strtok(NULL, " ");
	}
	
	ex = strtok(ex, "\n");

	pid_t pid = fork();
	if (pid==0){
		printf("child executing\n");
		char *cmd[]={ex, NULL};
		
		int val = execv(cmd[0],cmd);
		if (val == -1){
			printf("error with execv\n");
			exit(1);
		}
	}		
	else if (pid > 0){
		printf("parent waiting for child\n");
		waitpid(pid,0,0);
		printf("parent done waiting\n");
		
	}
	else{
		printf("error\n");
	}	

}


int parseInput(char *input, char *history[], int *haddr, char* token, bool *waddr ){ 
//parseInput Function
//passed in the buffer from main
//buffer then parsed and proper funcions called
//this will be useful when calling functions from history
//this will handle the input checks aswell
//  if history than call history controller
//  if cd then change directory, might have to be static variable
//  if an executable then call executeprog (which then forks a process
//  if exit then return -1
//should return -1 if the input is exit which will trigger main to return 0
//otherwise it will return 0 and 
//history

	token = strtok(input, " ");

	if (strncmp(token, "history", 6)==0){
		printf("input is history\n");
		historyController(history, haddr, token, waddr);		
	}
	else if(strncmp(token, "cd", 1) == 0){
		printf("input is cd\n");
        char* directory = strtok(NULL, " ");
        directory = strtok(directory, "\n");
        changeDirectory(directory);
    }
	else if (strncmp(token, "exit", 3) != 0){
		printf("input is not history, cd, or exit\n");	
		executeProg(token);
	}
	else{
		printf("input is exit\n");
		return -1;
	}
	
//	printf("inside parse input \n");
//	printf("%s\n", input);
	return 0;
}

void changeDirectory(char* directory)
{
	//printf("this is the directory: %s", directory);
    DIR* dir;
    if ((dir = opendir(directory)) != NULL){
        closedir(dir);
        chdir(directory);
    }
    else if(ENOENT==errno)
    {
        printf("Directory '%s' does not exist\n", directory);
    }
    else{
        printf("opendir() failed for some other reason\n");
    }
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

	int parseVal = 0;


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
			
		parseVal = parseInput(buffer, history, &histInd, token, &hasWrapped);
			
		//tokenizing input
		}while(parseVal >= 0);

	return 0;
	
}


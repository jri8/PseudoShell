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
#include <sys/stat.h>
#include <fcntl.h>
/*----------TO-DO-----------
//fix executable check
//implement functionality for multiple pipes
------------TO-DO----------*/

void changeDirectory(char*);
bool isDirectory(char*);

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
            if (cmdInd > histInd && hasWrapped == false)
                printf("error: %s\n", "offset is not valid");
            else if(cmdInd < MAX_HISTORY && hasWrapped == true){
                parseInput(history[(histInd+cmdInd-1)%MAX_HISTORY], history, haddr, token, waddr);
            }
            else {
                printf("Command to be executed: %s", history[cmdInd]);
			    printf("executing history cmd: %d\n", cmdInd);
//                printf("token=%s", token);
                parseInput(history[cmdInd],history,haddr,token,waddr);
//              cancels carriage return when history prints again, needs to be fixed            
            }
        }
		//not a valid modifier
		else{
			printf("error: %s\n", "not a valid modifier");
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
	
	char *progs[100];
	char *args[100][100];
    int argcount = 0;
    char *fname;
	int fout, fin;
	int saved_stdout = dup(1);
	int saved_stdin = dup(0);
	int execs = 0;
	progs[execs] = malloc(strlen(token)+1);
	progs[execs] = token;
	execs++;
	
	bool sendBack = false;
	bool piped = false;
//	printf("in execProg\n");
//	printf("%s\n", progs[0]);

	int mypipe[2];
	if (pipe(mypipe)){
		printf("error: %s\n", "pipe failed");
	}	
	
	token = strtok(NULL, " ");
	while (token != NULL){
		if(strncmp(token, "|", 1) == 0){
			printf("pipe\n");
			token = strtok(NULL, " ");
			progs[execs] = malloc(strlen(token)+1);
			progs[execs] = token;
            		argcount = 0;
			execs++;
			piped = true;
		} 
		else if(strncmp(token, ";", 1) == 0){
//			printf("run second program\n");
			token = strtok(NULL, " ");
			progs[execs] = malloc(strlen(token)+1);
			progs[execs] = token;
            argcount = 0;
			execs++;
		}
		else if (strncmp(token, "<", 1) == 0){
			printf("redirect in\n");
			token = strtok(NULL, " " );
			token = strtok(token, "\n");
			fname = token;
			fin = open(fname, O_RDONLY);
			if(-1 == dup2(fin, 0)) 
				printf("error: %s\n", "dup2() failed to execute");
			printf("leaving < block \n");
		    
        	}
		else if (strncmp(token, ">", 1) == 0){
			printf("redirect out\n");
			token = strtok(NULL, " ");
			token = strtok(token, "\n");
			fname = token;
			fout = open(fname, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
			if(-1 == dup2(fout, 1))
				printf("error: %s\n", "dup2() failed to execute");
			printf("leaving > block \n");
			
		}
		else if (strncmp(token, "&", 1) == 0){
			printf("send process to background\n");
			sendBack = true;	
		}
		else{
			//printf("%s\n", token);
			if (strchr(token, 10) != NULL)
				token = strtok(token,"\n");
			args[execs-1][argcount] = malloc(strlen(token)+1);
			args[execs-1][argcount] = token;
			printf("%s\n",args[execs-1][argcount]);
			argcount++;
		}
		
		token = strtok(NULL, " ");
	}
	
	pid_t pid;
	int i = 0;
	for(i; i < execs; i++){	
		progs[i] = strtok(progs[i], "\n");
		
		if ((pid = fork()) ==0){
			if (piped){
				if (i == 0){
					printf("in first proc block\n");
					printf("i = %d\n", i);
					printf("redirecting output\n");
					if (-1 == dup2(mypipe[1], 1))
					    printf("error: %s\n", "dup2() failed to execute");
					close(mypipe[0]);
				}
				else if (i == execs - 1){	
					printf("in second proc block\n");
					printf("i = %d\n", i);
					printf("redirecting input\n");
					if (-1 == dup2(mypipe[0], 0))
						printf("error: %s\n", "dup2() failed to execute");
					close(mypipe[1]);
				    dup2(saved_stdout,1);
                }
				else{
					printf("in third proc block\n");
					printf("i = %d\n", i);
					printf("redirecting input\n");
					if (-1 == dup2(mypipe[0], 0))
						printf("error: %s\n", "dup2() failed to execute");
					printf("redirecting output\n");
					if (-1 == dup2(mypipe[1], 1))
						printf("error: %s\n", "dup2() failed to execute");
				}
			}

	        int g = 0;
            char *cmd[100];
            cmd[0] = progs[i];
//            printf("args[%d][%d]=%s\n",i,g,args[i][g]);
            while(args[i][g] != NULL){
                cmd[g+1] = args[i][g];
//                printf("cmd[g+1]=%s\n",cmd[g+1]);
                g++;
            }
			cmd[g+1] = NULL;
            int val = execv(cmd[0],cmd);
			if (val == -1){
				printf("error: %s\n", "failed execution w/ execv()");
				exit(1);
			}
			
		}		
		else if (pid > 0){
			printf("in parent block\n");
			if (i == execs-1){
				printf("closing parent pipes\n");
				close (mypipe[0]);
				close (mypipe[1]);
			}
			
			if (!sendBack){
				//printf("parent waiting for child\n");
				waitpid(pid,0,0);
			//	printf("parent done waiting\n");
			}
		}
		else{
			printf("error: %s\n","unknown error");
		}
	}
	close(mypipe[0]);
	close(mypipe[1]);
	dup2(saved_stdout, 1);
	dup2(saved_stdin, 0);
	close(saved_stdout);
	close(saved_stdin);	

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
	//printf("%s = input in parseInput\n", input);
	token = input;
	token = strtok(token, " ");
    	struct stat sb;

	if (strncmp(token, "history", 7)==0){
		printf("input is history\n");
		historyController(history, haddr, token, waddr);		
	}
	else if(strncmp(token, "cd", 2) == 0){
		printf("input is cd\n");
        	char* directory = strtok(NULL, " ");
       		directory = strtok(directory, "\n");
       		changeDirectory(directory);
    	}

	else if (strncmp(token, "exit", 4) != 0){
		printf("input is not history, cd, or exit\n");
        //char* exec = strtok(token, "\n");
        //bool isFolder = false;
        //isFolder = isDirectory(token);
	    //if(stat(exec,&sb)== 0 && sb.st_mode & S_IXUSR && isFolder == false)
        //{
         //   printf("the file '%s' is an executable\n", token);
            executeProg(token);
        //}
        //else{
        //    printf("error: %s\n", "invalid input/not an executable");
        //}
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
        printf("error: %s\n","Directory does not exist");
    }
    else{
        printf("error: %s\n", "failed to open directory");
    }
}

bool isDirectory(char* directory)
{
    bool isDir = false;    
    DIR* dir;
    if ((dir = opendir(directory)) != NULL){
        isDir = true;
        closedir(dir);
//        chdir(directory);
    }
    else if(ENOENT==errno)
    {
//        printf("error: %s\n","Directory does not exist");
    }
    else{
//        printf("error: %s\n", "failed to open directory");
    }
    return isDir;
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


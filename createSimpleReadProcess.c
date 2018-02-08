//causes program 1 to be executed

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
int main(int argc, char* argv[])
{

	pid_t pid = fork();
	if (pid==0){
		char *cmd[]={"helloWorld", NULL};
//		printf("if statement\n");
		int val = execv(cmd[0],cmd);
		if (val == -1)
			exit(1);
	}		
	
	else{
		waitpid(pid,0,0);
	}	
//	printf("returning\n");
	return 0;	

}

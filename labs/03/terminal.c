#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

//it works with ls and pwd , it doesn't work with commands that need another argument

#define COMMAND_MAX_SIZE 4    // ls, pwd, (characters + null terminator)
#define NULLL 0
#define LF 10

int main(int argc, char const *argv[]) {

	pid_t pid;
	char count;
	char arg[COMMAND_MAX_SIZE];

	for(;;){

		printf("#");
		if ( fgets (arg , COMMAND_MAX_SIZE , stdin) == NULL ) {
			printf("error while getting string from user\n");
			return 1;
		}
		//see how long is the arg
		count = 0;
		char i;
		for(i=0; i<COMMAND_MAX_SIZE; i++){
			if(arg[i]!=NULLL && arg[i]!=LF){
				count++;
			}
		}
		//make the arrays with no extra spaces
		char exact_arg[count+1];
		strncpy(exact_arg, arg, count);
  	exact_arg[count] = '\0';
		char command[6+count];
		strcpy(command,  "/bin/");
		strcat(command, exact_arg);


		/* fork a child process */
		pid = fork();
		if (pid < 0) {
			/* error occurred */
			fprintf(stderr, "Fork Failed");
			exit (-1) ;
		}

		else if (pid == 0) {
			/* child process */
			execlp(command, exact_arg, NULL);
		}

		else {
			/* parent process */
			/* parent will wait for the child to complete */
			wait(NULL);
			//reset variables before next iteration
			memset(&arg[0], 0, sizeof(arg));
			memset(&command[0], 0, sizeof(command));
			count = 0;
		}
	}

}

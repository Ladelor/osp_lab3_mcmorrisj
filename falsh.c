#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char* argv[])
{
	//This means the user entered falsh -h or someother bad input
	if(argc == 2)
	{
		//If they did falsh -h, print help.txt and exit
		if(strstr(argv[1], "-h") != 0)
		{
			help();
			return 0;
		}
		//Otherwise, invalid call
		else
		{
			printf("Invalid falsh call, no parameters or -h flag only\n");
			return -1;
		}
	}
	//If there are more than two args, it was an invalid call
	else if(argc > 2)
	{
		printf("Invalid falsh call, no parameters or -h flag only\n");
		return -1;
	}

	//setenv sets an environment variable
	//Passing in that we want to change "PATH"
	//we are chaning it to "\bin", and we are overriding it
	setenv("PATH", "/bin", 1);

	//Some variables to deal with keeping track of stdout and strerr with redirection
	int fdOUT = -1, fdERR = -1;
	//dup copies a file descriptor are returns a copy of it
	int OUTCopy = dup(STDOUT_FILENO);
	int ERRCopy = dup(STDERR_FILENO);

	//Program will exit on user exit command
	while(1)
	{
		//Assuming max input length is 256
		int maxLength = 256;
		//cstring to store user entry
		char userInput[maxLength];
		//Just a boring message for good looks
		printf("Welcome to the best shell! >> ");
		//fgets reading from a filestream for some length and storing that string
		//First param is buffer to store what is read
		//Second param is the max length to read (we are assuming 256)
		//Third param is what to read from, stdin
		fgets(userInput, maxLength, stdin);

		//This means only an enter was hit, just prompt the user again
		if(strlen(userInput) == 1)
			continue;

		//Copying the input as strtok messes with the string given
		char *userInputCopy = NULL;
		//duplicate the userInput and store in a copy
		userInputCopy = strdup(userInput);
		//char** to store args like argv
		//Max 100 args
		char *args[100];
		//Parameter to store arg count like argc
		int argCount = 0;
		//Go through the string copy until there are no more tokens
		//Returns a token, stores in args, and adds to argCount
		//Ends when there are no more tokens
		while((args[argCount] = strtok(userInputCopy, " ")) != NULL)
		{
			argCount++;
			userInputCopy = NULL;
		}
		//Free the memory created by the strdup to copy userInput
		free(userInputCopy);

		//Without this, have an excess new line at the end of the last argument
		//Go to the last argument, get the length, replace the length - 1 spot with a null term
		//In effect, this replaces the unwanted \n with a \0
		args[argCount - 1][strlen(args[argCount - 1]) - 1] = '\0';

		//Search through each arg to see if we will redirect
		for(int i = 0; i < argCount; i++)
		{
			//This means we are going to attempt to redirect
			if(strstr(args[i], ">") != 0)
			{
				//This means there is exactly one arg after >
				//This is a valid redirect
				if(i == (argCount - 2))
				{
					//Copy the argument for convenience
					//In adding .out and .err
					char* fileCopy = strdup(
						args[argCount-1]);
					//Closing stdout
					close(STDOUT_FILENO);
					//opening userparam + .out
					fdOUT = open(strcat(fileCopy,".out"),
					O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
					//closing stderr
					close(STDERR_FILENO);
					//opening userparam + .err
					//Recopying to get rid of .out
					strcpy(fileCopy, args[argCount - 1]);
					fdERR = open(strcat(fileCopy,".err"),
					O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
					//free the copy made by strdup
					free(fileCopy);
					//These lines make sure the correct args
					//are passed to any program we exec
					//i.e. get rid of the > and filePath
					args[argCount - 1] = NULL;
					args[argCount - 2] = NULL;
					argCount -=2;
				}
				//This means there are more than one params
				//after the >
				else
				{
					printf("Invalid use of redirection \
only one file argument allowed\n");
					//Cheap and dirty way to identify
					//the invalid redirection
					argCount = -1;
				}
			}
		}

		//Redirection failed, leave
		if(argCount == -1)
			continue;

		//Ignoring extra params on most commands
		//Noticed bash did this in some cases so didn't check everything
		//User entered exit so exit
		if(strstr(args[0], "exit") != 0)
		{
			exitProgram();
		}
		//User entered pwd, so just run pwd
		else if(strstr(args[0], "pwd") != 0)
		{
			pwd();
		}
		//User entered cd, if there are no args, move to HOME
		//One arg, try and move to it
		//Two+ args, print an error message
		else if(strstr(args[0], "cd"))
		{
			if(argCount == 1)
				cd(NULL);
			else if(argCount == 2)
				cd(args[1]);
			else
				printf("cd only accepts one argument\n");
		}
		//setpath has little to no error handling as per instructions
		//It will make sure there is atleast one arg
		//Give it effectively argc, argv
		else if(strstr(args[0], "setpath"))
		{
			setpath(argCount, args);
		}
		//User entered help, prints help.txt
		else if(strstr(args[0], "help"))
		{
			help();
		}
		//We will try to run any other input as an exec
		else
		{
			//Fork a child process
			int rc = fork();
			//if rc is 0, the fork failed and something is bonked
			//This is an error beyond our handling so just exit with error
			if(rc < 0)
			{
				fprintf(stderr, "Fork failed\n");
				exit(1);
			}
			//This is the child proccess
			//Attempt to exec it
			else if(rc == 0)
			{
				//Mostly using execvp to follow example from book
				//Attempt to call the program with given args
				//This will not return unless it can't run args[0]
				execvp(args[0], args);
				//Print the error message from the child
				printf("%s command not found\n", args[0]);
				//Exit the child allowing the parent to continue
				exit(1);
			}
			else
			{
				//Wait for the child to finish the exec or exit after failing to exec
				wait(NULL);
			}
		}

		//This means we did redirection
		if(fdOUT != -1)
		{
			//Close what we were redirecting stdOUT to
			close(fdOUT);
			//Copy stdouts fd back into the right slot
			dup2(OUTCopy, STDOUT_FILENO);
			//reset for the next command
			fdOUT = -1;
			//Same process as above for stderr
			close(fdERR);
			dup2(ERRCopy, STDERR_FILENO);
			fdERR = -1;
		}
	}
	//This should never actually run
	return 0;
}

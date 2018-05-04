#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
//#include <sys/stat.h>

int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		if(strstr(argv[1], "-h") != 0)
		{
			help();
			return 0;
		}
		else
		{
			printf("Invalid falsh call, no parameters or -h flag only\n");
			return -1;
		}
	}
	else if(argc > 2)
	{
		printf("Invalid falsh call, no parameters or -h flag only\n");
		return -1;
	}

	//Reset PATH to only include /bin
	setenv("PATH", "/bin", 1);

	while(1)
	{
		//Assuming max input length is 256
		int maxLength = 256;
		//cstring to store user entry
		char userInput[maxLength];
		printf("The best shell! >> ");
		fgets(userInput, maxLength, stdin);

		char *userInputCopy = NULL;
		userInputCopy = strdup(userInput);
		char *args[100];
		int argCount = 0;
		while((args[argCount] = strtok(userInputCopy, " ")) != NULL)
		{
			argCount++;
			userInputCopy = NULL;
		}
		free(userInputCopy);

		//Without this, have an excess new line at the end of the last argument
		//Go to the last argument, get the length, replace the length - 1 spot with a null term
		//In effect, this replaces the unwanted \n with a \0
		args[argCount - 1][strlen(args[argCount - 1]) - 1] = '\0';

		int fdOUT, fdERR = -1;
		int OUTCopy = dup(STDOUT_FILENO);
		int ERRCopy = dup(STDERR_FILENO);
		if(argCount > 2)
		{
			char* fileCopy = strdup(args[argCount - 1]);
			if(strstr(args[argCount - 2], ">") != 0)
			{
				close(STDOUT_FILENO);
				fdOUT = open(strcat(fileCopy,".out"),
					O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
				close(STDERR_FILENO);
				strcpy(fileCopy, args[argCount - 1]);
				fdERR = open(strcat(fileCopy,".err"),
					O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			}
			free(fileCopy);
		}

		if(strstr(args[0], "exit") != 0)
		{
			exitProgram();
		}
		else if(strstr(args[0], "pwd") != 0)
		{
			char buf[256];
			pwd(buf);
			printf("%s\n", buf);
		}
		else if(strstr(args[0], "cd"))
		{
			if(argCount == 1)
				cd(NULL);
			else if(argCount == 2)
				cd(args[1]);
			else
				printf("cd only accepts one argument\n");
		}
		else if(strstr(args[0], "setpath"))
		{
			setpath(argCount, args);
		}
		else
		{
			int rc = fork();
			if(rc < 0)
			{
				fprintf(stderr, "Fork failed\n");
				exit(1);
			}
			else if(rc == 0)
			{
				execvp(args[0], args);
				printf("%s failed to execute\n", args[0]);
				exit(1);
			}
			else
			{
				wait(NULL);
			}
		}

		if(fdOUT != -1)
		{
			close(fdOUT);
			dup2(OUTCopy, STDOUT_FILENO);
			close(OUTCopy);
			fdOUT = -1;
			close(fdERR);
			dup2(ERRCopy, STDERR_FILENO);
			close(ERRCopy);
		}
	}
	//printf("hi\n");
	return 0;
}

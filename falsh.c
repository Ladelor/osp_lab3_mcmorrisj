#include <stdio.h>
#include <string.h>
#include "commands.h"
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
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

		//Without this, have an excess new line at the end of the last argument
		//Go to the last argument, get the length, replace the length - 1 spot with a null term
		//In effect, this replaces the unwanted \n with a \0
		args[argCount - 1][strlen(args[argCount - 1]) - 1] = '\0';

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
			printf("Nope\n");
	}
	printf("hi\n");
	return 0;
}

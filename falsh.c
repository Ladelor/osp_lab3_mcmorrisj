#include <stdio.h>
#include <string.h>
#include "commands.h"

int main(int argc, char* argv[])
{
	while(1)
	{
		//Assuming max input length is 256
		int maxLength = 256;
		//cstring to store user entry
		char userInput[maxLength];
		fgets(userInput, maxLength, stdin);
		if(strstr(userInput, "exit") != 0)
		{
			exitProgram();
		}
		else
			printf("Nope\n");
	}
	printf("hi\n");
	return 0;
}

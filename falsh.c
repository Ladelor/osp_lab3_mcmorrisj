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
		printf("The best shell! >> ");
		fgets(userInput, maxLength, stdin);
		if(strstr(userInput, "exit") != 0)
		{
			exitProgram();
		}
		else if(strstr(userInput, "pwd") != 0)
		{
			char buf[256];
			pwd(buf);
			printf("%s\n", buf);
		}
		else if(userInput[0] == 'c' && userInput[1] == 'd')
		{
//			cd(argc, argv);
		}
		//else
		//	printf("Nope\n");
	}
	printf("hi\n");
	return 0;
}

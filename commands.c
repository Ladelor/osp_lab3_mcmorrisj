#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
int exitProgram()
{
	//Terminating process with success
	//Exit kills the process and returns the integer argument to the parent
	//An argument of 0 indicates we have exited with success
	exit(0);

	//According to the man (and what it should do), exit does not return
	//I don't know if there is value in handling the case it fails so just return failure
	return -1;
}

int pwd()
{
	char buf[256];
	//getcwd returns the current working directory
	//buf is the char* which is passed in as a parameter
	//this array will be set to a NULL terminated char array of the current working directory
	//the second param is an integer containing the max read length
	//If the cwd is greater than 256, it will return what is has read so far and null terminate it
	//the return value is the same as what is stored in buf
	//If the return is NULL, something went wrong so we will report that
	if(getcwd(buf, 256) == NULL)
	{
		fprintf(stdout, "Failed to get current working directory\n");
		return -1;
	}
	//Succesful completion of cwd
	printf("%s\n", buf);
	return 0;
}

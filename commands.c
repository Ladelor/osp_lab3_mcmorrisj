
#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

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

int pwd(char* buf)
{
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
	return 0;
}

int cd(char* filePath)
{
	if(filePath == NULL)
	{
		if(chdir(getenv("HOME")))
		{
			fprintf(stderr, "Failed to change directory\n");
			return -1;
		}
	}
	else
	{
		if(chdir(filePath))
		{
			fprintf(stderr, "Failed to change directory\n");
			return -1;
		}
	}
	return 0;
}

//Not letting user setpath to an invalid filepath
int setpath(int argc, char* argv[])
{
	if(argc == 1)
	{
		fprintf(stderr, "Failed to set path, must have atleast one filePath parameter\n");
		return -1;
	}

	//Storing old path in case there is a problem and we need to restore the old
	//Is this a memory leak if I don't free?
	char* oldPath = strdup(getenv("PATH"));
	//Remove the values in PATH
	setenv("PATH", "", 1);

	for(int i = 1; i < argc; i++)
	{
		DIR* directory = opendir(argv[i]);
		//This means argv[i] is a valid filePath
		if(directory)
		{
			if(i > 1)
				setenv("PATH", strcat(getenv("PATH"), ":"), 1);
			setenv("PATH", strcat(getenv("PATH"), argv[i]), 1);
			closedir(directory);
		}
		else
		{
			//I'm not going to free here because I don't think setenv copies
			setenv("PATH", oldPath, 1);
			fprintf(stderr, "Failed to setpath, %s was not \
 a valid directory\n", argv[i]);
			return -1;
		}
	}
	free(oldPath);
	printf("PATH set to: %s\n", getenv("PATH"));
	return 0;
}

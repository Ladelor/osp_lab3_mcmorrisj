#include "commands.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>

//Function to exit falsh
//No parameters, no return, just exits
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

//Function to print the working directory
//No parameters
//Returns 0 on success, -1 when getcwd fails
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

	//Print the current wd and return success
	printf("%s\n", buf);
	return 0;
}

//Function to change the current directory
//Paramter is directory to change to
//If filePath is NULL, move to the HOME directory
//Returns 0 on success, -1 on failure to change directory
int cd(char* filePath)
{
	//This means we are moving to HOME
	if(filePath == NULL)
	{
		//getenv("HOME") will return a pointer to the cstring holding the home directory
		//chdir uses that cstring and changes the directory
		//If it returns 0, we're good, otherwise, something went wrong
		if(chdir(getenv("HOME")))
		{
			fprintf(stderr, "Failed to change directory\n");
			return -1;
		}
	}
	//This means we will attempt to change the directory to filePath
	else
	{
		//If it returns 0, we're good, otherwise, something went wrong
		if(chdir(filePath))
		{
			fprintf(stderr, "Failed to change directory\n");
			return -1;
		}
	}
	return 0;
}

//Function to set the PATH env variable
//Paramters are the effective argc and argv of the user input
//Returns 0 on success, -1 when there was no argument for setpath
//letting user setpath to an invalid filepath
int setpath(int argc, char* argv[])
{
	//This means setpath was called with no further argument
	if(argc == 1)
	{
		fprintf(stderr, "Failed to set path, must have atleast one filePath parameter\n");
		return -1;
	}

	//Remove the values in PATH
	//Takes the env var to change: PATH
	//What to change it to: "" (empty)
	//And if it should overwrite (yes)
	setenv("PATH", "", 1);

	//Iterate through each argument after the setpath
	for(int i = 1; i < argc; i++)
	{
		//If we are adding more than one path, we need to add : to make the list
		if(i > 1)
			//Sets PATH to what PATH was and appends a :
			setenv("PATH", strcat(getenv("PATH"), ":"), 1);
		//Appends this setpath argument to PATH
		setenv("PATH", strcat(getenv("PATH"), argv[i]), 1);
	}
	//Prints what PATH has been momdified to
	printf("PATH set to: %s\n", getenv("PATH"));
	return 0;
}

//Function to print the help.txt file with use instructions
//No parameters, always returns success unless help.txt is not found
int help()
{
	//open the file, store the stream in file
	FILE* file = fopen("help.txt", "r");
	//If it fails to open, print the error and return -1
	if(!file)
	{
		printf("help.txt file not found!\n");
		return -1;
	}

	//Buffer to store text from the file
	char buf[256];
	//Iterate through each line of the file assuming max length of 256
	while(fgets(buf, 256, file))
		printf("%s", buf);
	//Just for extra whitespace after printing help.txt
	printf("\n");
	return 0;
}

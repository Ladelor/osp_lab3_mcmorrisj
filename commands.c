#include "commands.h"
#include <stdlib.h>

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

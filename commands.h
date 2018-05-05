//pragma once is the same as the #ifndef, #define, #endif sequence
//Fewer lines, less likely for mistakes, imo it should be standard now
#pragma once

//Function headers, more info can be found in help.txt

//Not named exit to avoid conflict built-in function warning
int exitProgram();
int pwd();
int cd(char*);
int setpath(int, char**);
int help();

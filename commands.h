//pragma once is the same as the #ifndef, #define, #endif sequence
//Fewer lines, less likely for mistakes, imo it should be standard now
#pragma once

//Not named exit to avoid conflict built-in function warning
int exitProgram();
int pwd(char*);
int cd(char*);
int setpath(int, char**);
int help();

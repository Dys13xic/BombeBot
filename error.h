#ifndef ERROR_H
#define CONFIGURE_H

#include <stdbool.h>

#define S_SUCCESS 0
#define S_INVALID_FLAG 1
#define S_FAILED_MALLOC 2
#define S_INVALID_RANGE 3
#define S_INVALID_CHARACTER 4
#define S_INVALID_LENGTH 5
#define S_INVALID_SUBARGUMENT_NUMBER 6
#define S_INVALID_SUBARGUMENT_REPETITION 7


void printUsage(char *programName, bool error);

void printError(int errorCode, char *functionName);

void errorExit(int errorCode, char *functionName, char *programName, bool error);

#endif
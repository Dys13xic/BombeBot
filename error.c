#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

void printUsage(char *programName, bool error) {

    char usageMessage[] = "Usage: %s [OPTION]... MESSAGE\n"
                    "Convert MESSAGE by simulating Enigma I cipher machine.\n"
                    "\n"
                    "Required:\n"
                    "   MESSAGE: The input to be converted, must be enclosed in quotes if it contains spaces.\n"
                    "               (Example: \"this is a message to convert\")\n"
                    "Options:\n"
                    "   -h, --help              Outputs usage and options information.\n"
                    "   -o, --ring              Ring settings for each rotor, range from 1-26 (inclusive).\n"
                    "                                   (Example: 13,1,26 - Default: 1,1,1)\n"
                    "   -p, --plugboard         Plugboard connects letters in pairs, swapping them (10 max), no duplicates.\n"
                    "                                   (Example: az,gh,lk - Default: empty)\n"
                    "   -r, --rotor             Rotor choices (I,II,III,IV,V) no duplicates.\n"
                    "                                   (Example: V,I,IV - Default: I,II,III)\n"
                    "   -s --start              Start position for each rotor, range from 1-26 (inclusive).\n"
                    "                                   (Example: 23,26,1 - Default: 1,1,1)\n"
                    "\n";

    if (error)
        fprintf(stderr, usageMessage, programName);

    else
        fprintf(stdout, usageMessage, programName);
}

void printError(int errorCode, char *functionName) {

    char *errorMessages[] = {
                            "",
                            "An invalid option flag was input",
                            "Failed to allocate memory",
                            "A number outside the valid range was input",
                            "An invalid character was input",
                            "An argument of invalid length was input",
                            "An invalid number of sub-arguments was input",
                            "Invalid repetition of a sub-argument"
                            };

    printf("ERROR in %s: %s.\n\n", functionName, errorMessages[errorCode]);
}

void errorExit(int errorCode, char *functionName, char *programName, bool error) {
    printError(errorCode, functionName);
    printUsage(programName, printError);
    exit(EXIT_FAILURE);
}
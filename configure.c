#include <ctype.h>

#include "enigma.h"
#include "configure.h"
#include "error.h"


// takes an array of integers and an array of ROTORs
// fills the rotorSet array with the ROTOR objects specified by the rotorModels array
// returns an integer status code
int configureRotors(int rotorModels[], ROTOR rotorSet[]) {
    int i;

    // array of rotor cipher alphabets
    char cipherAlphabets[5][27] = {
                                {"EKMFLGDQVZNTOWYHXUSPAIBRCJ"},
                                {"AJDKSIRUXBLHWTMCQGZNPYFVOE"},
                                {"BDFHJLCPRTXVZNYEIWGAKMUSQO"},
                                {"ESOVPZJAYQUIRHXLNFTGKDCMWB"},
                                {"VZBRGITYUPSDNHLXAWMJQOFECK"},
    };

    // for each rotor in rotors array
    for (i = 0; i < ROTOR_AMOUNT; i++) {
        strncpy(rotorSet[i].cipherAlphabet, cipherAlphabets[rotorModels[i]], ALPHA_LENGTH + 1);    // set ROTOR object's corresponding cipher alphabet
        rotorSet[i].position = 0;
        rotorSet[i].turnover = 24;  // Bombe machine fast and middle rotor turnover on Y
    }

    return S_SUCCESS;
}


// splits the string str by comma delimiter into an array of strings
// returns pointer to array of strings
char **splitArg(char *str, int *size) {
    // allocating space to array of strings
    char **args = calloc(MAX_ARG_SEGMENTS, sizeof(char*));
    int i = 0;

    // initializing ptr to first string token
    char *p = strtok (str, ",");

    // while more string tokens exist
    while (p != NULL && i < MAX_ARG_SEGMENTS) {

        // ensures no string segment exceeds the maximum length of 3
        if (strlen(p) > ARG_SEGMENT_LENGTH) {
            printf("ERROR: an argument segment exceded the maximum length.\n");
            exit(1);
        }

        args[i] = (char *) malloc (ARG_SEGMENT_LENGTH + 1);         // allocate space in array of strings
        args[i] = p;                                       // set string value to current string token
        p = strtok (NULL, ",");                           // get next token
        i++;
    }

    // keep track of elements in dynamic array
    *size = i;

    if (p) {
        printf("ERROR: an argument exceded the maximum argument length.\n");     //TODO, handle error, remember to free the memory when it's done
        exit(1);
    }

    return args;
}


// Checks the input for valid number of rotors, no duplicates, and valid rotor models
int validateRotors(char **arg, int rotorSettings[], int size) {

    if (size != ROTOR_AMOUNT)
        return S_INVALID_LENGTH;

    char *upperNumeral[5] = {"I", "II", "III", "IV", "V"};
    char *lowerNumeral[5] = {"i", "ii", "iii", "iv", "v"};
    int unique[5] = {1, 1, 1, 1, 1};
    int i;

    // compares each argument to the roman numeral strings
    for (i = 0; i < ROTOR_AMOUNT; i++) {

        int j = 0;
        while (j < NUM_ROTOR_OPTIONS && strcmp(arg[i], upperNumeral[j]) != 0 && strcmp(arg[i], lowerNumeral[j]) != 0)
            j++;

        // if the argument matches a numeral (and that numeral hasn't been used before) the argument is converted to an integer
        if (j < NUM_ROTOR_OPTIONS && unique[j]) {
            rotorSettings[i] = j;
            unique[j] = 0;
        }

        // if the argument doesn't match a numeral, or that numeral has already been taken, exit with error message
        else {
            if (j < NUM_ROTOR_OPTIONS && !unique[j])
                return S_INVALID_SUBARGUMENT_REPETITION;
            else
                return S_INVALID_CHARACTER;
        }
    }

    // call rotorSet to finalize rotors
    return S_SUCCESS;
}


// Swaps first instance of characters x and y in a string.
void swapChars(char *string, char x, char y) {
    char temp;
    
    // get pointer to first instance of char in string
    char *ptr1 = strchr(string, x);
    char *ptr2 = strchr(string, y);

    temp = *ptr1;

    // changes the pointer values
    *ptr1 = *ptr2;
    *ptr2 = temp;
}


// returns plugboardCipher Alphabet
int validatePlugboard(char **arg, char plugboardCipher[], int size) {

    // error case
    if (size > PLUG_LIMIT) {
        return S_INVALID_LENGTH;
    }    

    char plugBoardArray[PLUG_LIMIT][2] = {};
    int unique[ALPHA_LENGTH] = {0};
    int i, j;

    // for each argument segment
    for (i = 0; i < size; i++) {

        // check there are two characters
        if (strlen(arg[i]) != 2) {
            return S_INVALID_SUBARGUMENT_NUMBER;
        }

        // for each character in pair
        for (j = 0; j < 2; j++) {

            // try to uppercase the character
            arg[i][j] = toupper(arg[i][j]);

            // if character is a valid uppercase letter, and hasn't been used yet
            if (arg[i][j] >= 'A' && arg[i][j] <= 'Z' && unique[arg[i][j] - 'A'] == 0) {

                plugBoardArray[i][j] = arg[i][j];       // build into plugboard 2d array
                unique[arg[i][j] - 'A'] = 1;            // set letter as used
            }

            // error cases
            else {

                if (unique[arg[i][j] - 'A'] != 0)
                    return S_INVALID_SUBARGUMENT_REPETITION;
                else
                    return S_INVALID_CHARACTER;
            }   
        }
    }

    i = 0;
    // swaps letters in plugBoardCipher with pairs in plugboardArray
    while (plugBoardArray[i][0]) {
        swapChars(plugboardCipher, plugBoardArray[i][0], plugBoardArray[i][1]);
        i++;
    }

    free(arg);                      // free argument memory space 
    return S_SUCCESS;         // return plugBoardCipher string
}


int validateNumericSettings(char **arg, int numericSettings[], int size) {

    if (size != ROTOR_AMOUNT)
        return S_INVALID_LENGTH;

    int numSetting, i;

    // for each argument segment
    for (i = 0; i < size; i++) {

        numSetting = atoi(arg[i]);

        if (numSetting || *arg[i] == '0') {

            if (numSetting >= 1 && numSetting <= 26)
                numericSettings[i] = numSetting - 1;     // offset by one so value works with index starting at 0
            else 
                return S_INVALID_RANGE;
        }

        else
            return S_INVALID_CHARACTER;
    }
    
    return S_SUCCESS;
}


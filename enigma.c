#include <ctype.h>
#include "enigma.h"
#include "configure.h"


// Searches for the first occurence of a character within a string
// Returns index of the first occuerence of character, or -1 if character not found.
static int strIndex(char* string, char character) {
    int index;
    char *ptr = strchr(string, character);

    if (ptr) 
        index = ptr - string;
    else
        index = -1;
    
    return index;
}


// Subtracts y from x, wrapping any result < 0 or any result >= mod around the mod
int moduloSubtract(int x, int y, int mod) {
    x = (x + (mod - y)) % mod;
    return x;
}


// Steps the rotors according to their current positions and turnover settings
void stepRotors(ROTOR *slowRotor, ROTOR *midRotor, ROTOR *fastRotor) {

    // step fastRotor
    fastRotor->position = (fastRotor->position + 1) % 26;

    // step midRotor if necessary (or condition handles enigma "double step")
    if (fastRotor->position == fastRotor->turnover || midRotor->position == moduloSubtract(midRotor->turnover, 1, ALPHA_LENGTH)) {
        midRotor->position = (midRotor->position + 1) % 26;
    }

    // step slowRotor if necessary (or conditon prevents constant increment of slowRotor when midRotor is double stepping)
    if (midRotor->position == midRotor->turnover && fastRotor->position == fastRotor->turnover + 1)
        slowRotor->position = (slowRotor->position + 1) % 26;
}


// applies rotor shift and returns the enciphrered letter
static char rotorCipher(ROTOR *rotor, char *alphabet, char letter) {
    int index, offsetIndex;

    index = letter - 'A';           // get the index of letter in alphabet
    offsetIndex = (index + rotor->position) % ALPHA_LENGTH;          // shift the letter index by rotor position
    char substitutionLetter = rotor->cipherAlphabet[offsetIndex];                 // update letter with substitution rotor alphabet
    
    index = substitutionLetter - 'A';                                   // get updated letter index from alphabet
    offsetIndex = moduloSubtract(index, rotor->position, ALPHA_LENGTH);      // reduce character value by subtracting rotation position from index
    char cipherLetter = (alphabet[offsetIndex]);
    
    return cipherLetter;
}


// returns enciphered letter
static char rotorCipherReverse(ROTOR *rotor, char *alphabet, char letter) {
    int index, offsetIndex;
    
    index = letter - 'A';                 // get letter index from alphabet
    offsetIndex = (index + rotor->position) % ALPHA_LENGTH;      // update letter, incrementing it by rotation position
    char offsetLetter = alphabet[offsetIndex];

    index = strIndex(rotor->cipherAlphabet, offsetLetter);       // find index of updated letter
    offsetIndex = moduloSubtract(index, rotor->position, ALPHA_LENGTH);
    char cipherLetter = alphabet[offsetIndex];

    return cipherLetter;
}

// Encodes a character according to Engima scrambler settings           //TODO fix this according to stripping out function below
char encodeChar(ROTOR rotorSet[], char letter) {
    if ( 'A' <= letter && letter <= 'Z') {
        char alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        int index, rotorIndex;

        //stepRotors(&rotorSet[SLOW_ROTOR], &rotorSet[MIDDLE_ROTOR], &rotorSet[FAST_ROTOR]);

        // rotors
        for (rotorIndex = FAST_ROTOR; rotorIndex >= SLOW_ROTOR; rotorIndex--) {
            letter = rotorCipher(&rotorSet[rotorIndex], alphabet, letter);
        }

        // reflector
        char reflectorB[27] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
        index = strIndex(alphabet, letter);
        letter = reflectorB[index];
        
        // rotors reverse
        for (rotorIndex = SLOW_ROTOR; rotorIndex <= FAST_ROTOR; rotorIndex++) {
            letter = rotorCipherReverse(&rotorSet[rotorIndex], alphabet, letter);
        }
        
        return letter;
    }
}


// runs the enigma machine on a given message
char *convertMessage(ROTOR rotorSet[], char *plugBoardCipher, char *inputMessage) {

    char *outputMessage = (char *) malloc(sizeof(char) * CHAR_LIMIT);
    int j = 0;

    while(inputMessage[j]) {
        outputMessage[j] = toupper(inputMessage[j]);
        j++;
    }
    outputMessage[j] = '\0';

    
    // encipher each letter in message
    int i = 0;
    while(outputMessage[i]) {

        char letter = outputMessage[i];
        if ( 'A' <= letter && letter <= 'Z') {
            char alphabet[27] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            int index, rotorIndex;

            stepRotors(&rotorSet[SLOW_ROTOR], &rotorSet[MIDDLE_ROTOR], &rotorSet[FAST_ROTOR]);

            // plugBoard entering
            index = strIndex(plugBoardCipher, letter);
            letter = alphabet[index];

            // rotors
            for (rotorIndex = FAST_ROTOR; rotorIndex >= SLOW_ROTOR; rotorIndex--)
                letter = rotorCipher(&rotorSet[rotorIndex], alphabet, letter);

            // reflector
            char reflectorB[27] = "YRUHQSLDPXNGOKMIEBFZCWVJAT";
            index = strIndex(alphabet, letter);
            letter = reflectorB[index];
            
            // rotors reverse
            for (rotorIndex = SLOW_ROTOR; rotorIndex <= FAST_ROTOR; rotorIndex++)
                letter = rotorCipherReverse(&rotorSet[rotorIndex], alphabet, letter);

            // plugBoard returning
            index = strIndex(plugBoardCipher, letter);
            letter = alphabet[index];
            
            outputMessage[i] = letter;
        }
        i++;
    }
    return outputMessage;
}
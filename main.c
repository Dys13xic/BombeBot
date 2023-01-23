#include "enigma.h"
#include "configure.h"
#include "error.h"
#include "list.h"
#include "graph.h"

#include <ctype.h>
#include <getopt.h>

int main(int argc, char *argv[]) {

    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char ciphertext[] = "RWIVTYRESXBFO";
    char crib[] = "WETTERVORHER";
    int length = 12;

    // TODO Reject if ciphertext and crib letters overlap in any position.

    // TODO Determine best menu programatically (for now we will not segment)
    // - Maximize loops
    // - Display menu in visual graph?
    // - Add command option to override relative position of ciphertext and crib


    // TODO create basic graph (with diagonal board connections)
    VERTEX valueMatrix[ALPHA_LENGTH][ALPHA_LENGTH];
    for (int c = 0; c < ALPHA_LENGTH; c++) {
        for (int w = 0; w < ALPHA_LENGTH; w++) {
            valueMatrix[c][w].val = false;
            append(&valueMatrix[c][w].adj, &valueMatrix[w][c]);
        }
    }

    // Create array for each double ended scrambler in battery
    ROTOR doubleEndedScrambler[BATTERY_SCRAMBLER_AMOUNT][ROTOR_AMOUNT];


    int rotorModels[3] = {1, 2, 3};
    
    for (int i = 0; i < length; i++) {
        configureRotors(rotorModels, doubleEndedScrambler[i]);
        // Initialize rotors to relative position
        doubleEndedScrambler[i][FAST_ROTOR].position = i;
    }

    // TODO determine additional connections for each letter in menu.
    for (int i = 0; i < length; i++) {
        int cipherCharIndex = ciphertext[i] - 'A';
        int cribCharIndex = crib[i] - 'A';

        for (int j = 0; j < ALPHA_LENGTH; j++) {
            char inputLetter = alphabet[j];
            char outputLetter = encodeChar(doubleEndedScrambler[i], inputLetter);
            int inputLetterIndex = inputLetter - 'A';
            int outputLetterIndex = outputLetter - 'A';


            // TODO ensure these are the valid indexes. 
            append(&valueMatrix[cipherCharIndex][inputLetterIndex], &valueMatrix[cribCharIndex][outputLetterIndex]);
            append(&valueMatrix[cribCharIndex][outputLetterIndex], &valueMatrix[cipherCharIndex][inputLetterIndex]);
        }
    }





    // Determine most common letter to connect to test register
    // - Merge sort
    // - Iterate through once counting and replacing letter depending on frequency.
    char testRegisterLetter = 'r';
    int testRegister = testRegisterLetter - 'a';

    return EXIT_SUCCESS;
}
#include "enigma.h"
#include "configure.h"
#include "error.h"

#include <ctype.h>
#include <getopt.h>

int main(int argc, char *argv[]) {

    char ciphertext[] = "RWIVTYRESXBFO";
    char crib[] = "WETTERVORHER";

    // determine best menu progaramatically

    // display menu in visual graph?

    // determine most common letter to connec to test register

    // build connections and initialize rotors
    // - diagional plugboard
    // - scramblers
    // - test register
    // - interconnections

    // run

    // test stop



    ROTOR battery[BATTERY_AMOUNT][ROTOR_AMOUNT];
    int i, j;

    int rotorSettings[ROTOR_AMOUNT] = {0,1,2};

    for (i = 0; i < BATTERY_AMOUNT; i++) {
        configureRotors(rotorSettings, battery[i]);
        battery[i]->position = i;
    }

    // Configure diagonal board
    WIRE *diagonalBoard[26][26];

    for (i = 0; i < ALPHA_LENGTH; i++) {
        for (j = 0; j < ALPHA_LENGTH; j++) {
            initWire(diagonalBoard[i][j]);
        }
    }

    for (i = 0; i < ALPHA_LENGTH; i++) {
        for (j = 0; j < ALPHA_LENGTH; j++) {
            if (i != j) {
                addWireOutput(diagonalBoard[i][j], diagonalBoard[j][i]);
            }
        }
    }

    // TODO Connect everything together
    


    // convert input and print output
    // char *output = convertMessage(rotorSet, plugboardCipher, input);

    // if (output) {
    //     printf("%s\n", output);
    //     free(output);
    //     output = NULL;
    // }

    return EXIT_SUCCESS;
}
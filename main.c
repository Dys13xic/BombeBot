#include "enigma.h"
#include "configure.h"
#include "error.h"

#include <ctype.h>
#include <getopt.h>


// Initializes input and output pointers to NULL and defaults current to false.
int initWire(WIRE *wire) {
    int i, length;
    length = sizeof(wire->output)/sizeof(wire->output[0]);

    // Initalize outputs to NULL
    for (i = 0; i < length; i++) {
        wire->output[i] = NULL;
    }

    wire->input = NULL;
    wire->current = false;

    return S_SUCCESS;
}

int addWireOutput(WIRE *source, WIRE *target) {
    int i, length;
    length = sizeof(source->output)/sizeof(source->output[0]);

    i = 0;
    while (i < length) {
        if (!source->output[i]) source->output[i] = target;
        i++;
    }

    return S_SUCCESS;
}



int main(int argc, char *argv[]) {

    char ciphertext[] = "RWIVTYRESXBFO";
    char crib[] = "WETTERVORHER";

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
#include "enigma.h"
#include "configure.h"
#include "error.h"
#include "list.h"
#include "graph.h"

#include <ctype.h>
#include <getopt.h>

void printGraph(VERTEX valueMatrix[26][26]) {
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    // Print value matrix
    printf("Value Matrix: \n");
    printf("-------------\n");
    printf("   ");

    char letter = 'a';
    for (int i = 0; i < ALPHA_LENGTH; i++) {
        printf("%c ", letter);
        letter += 1;
    }
    printf("\n");

    for (int cable = 0; cable < ALPHA_LENGTH; cable++) {
        printf("%c ", alphabet[cable]);
        for (int wire = 0; wire < ALPHA_LENGTH; wire++) {
            if(valueMatrix[cable][wire].val == true) {
                printf(" 1");
            } else {
                printf(" 0");
            }
        }
        printf("\n");
    }

    // Print adjacency lists
    printf("\nAdjacency Lists: \n");
    printf("----------------\n");
    for (int cable = 0; cable < ALPHA_LENGTH; cable++) {
        for (int wire = 0; wire < ALPHA_LENGTH; wire++) {
            printf("%c%c: ", alphabet[cable], tolower(alphabet[wire]));
            NODE *current = valueMatrix[cable][wire].adj.head;
            while(current != NULL) {
                for (int i = 0; i < ALPHA_LENGTH; i++) {
                    int j = 0;
                    for (j = 0; j < ALPHA_LENGTH; j++) {
                        if(current->data == &valueMatrix[i][j]) {
                            printf("%c%c, ", alphabet[i], tolower(alphabet[j]));
                            break;
                        }
                    }
                }
                current = current->next;
            }
            printf("\n");
        }
    } 
}

int main(int argc, char *argv[]) {

    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    char ciphertext[] = "RWIVTYRESXBFO";
    char crib[] = "WETTERVORHER";
    int length = 12;

    // TODO Reject if ciphertext and crib letters overlap in any position.
    // Or if length is different (until later when we programatically determine menu)
    // Or if length is greater than 12 (maybe add support for longer ciphers later)

    // TODO Determine best menu programatically (for now we will not segment)
    // - Maximize loops
    // - Display menu in visual graph?
    // - Add command option to override relative position of ciphertext and crib


    // Initialize graph vertices.
    VERTEX valueMatrix[ALPHA_LENGTH][ALPHA_LENGTH];
    LIST emptyList;
    emptyList.head = NULL;
    emptyList.tail = NULL;
    for (int cable = 0; cable < ALPHA_LENGTH; cable++) {
        for (int wire = 0; wire < ALPHA_LENGTH; wire++) {
            valueMatrix[cable][wire].val = false;
            valueMatrix[cable][wire].adj = emptyList;
        }
    }

    // Add diagonal board connections
    for (int cable = 0; cable < ALPHA_LENGTH; cable++) {
        for (int wire = 0; wire < ALPHA_LENGTH; wire++) {
            if (cable != wire) {
                append(&valueMatrix[cable][wire].adj, &valueMatrix[wire][cable]);
            }
        }
    }

    // Create array of double ended scramblers in battery
    ROTOR doubleEndedScrambler[BATTERY_SCRAMBLER_AMOUNT][ROTOR_AMOUNT];

    int rotorModels[3] = {1, 2, 3};
    
    for (int i = 0; i < length; i++) {
        // Initialize scrambler rotors and set relative position
        configureRotors(rotorModels, doubleEndedScrambler[i]);
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

            // TODO see if there is a way to halve letters we interate over.
            append(&valueMatrix[cipherCharIndex][inputLetterIndex].adj, &valueMatrix[cribCharIndex][outputLetterIndex]);
            //append(&valueMatrix[cribCharIndex][outputLetterIndex], &valueMatrix[cipherCharIndex][inputLetterIndex]);      Shouldn't be necessary if going through all letters in alphabet.
        }
    }

    printGraph(valueMatrix);




    // Determine most common letter to connect to test register
    // - Merge sort
    // - Iterate through once counting and replacing letter depending on frequency.
    // char testRegisterLetter = 'r';
    // int testRegister = testRegisterLetter - 'a';

    return EXIT_SUCCESS;
}
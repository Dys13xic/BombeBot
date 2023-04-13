#include "enigma.h"
#include "configure.h"
#include "error.h"
#include "list.h"
#include "graph.h"

#include <ctype.h>
#include <getopt.h>

void printGraph(VERTEX valueMatrix[ALPHA_LENGTH][ALPHA_LENGTH]) {
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

    char ciphertext[] = "RWIVTYRESXBF";
    char crib[] = "WETTERVORHER";
    int length = 12;

    // TODO Add support for cipher/crib pairs less than 12 characters

    // TODO Determine best menu programatically (for now we will not segment)
    // - Maximize length up to 12 characters
    // - No crashes
    // - Maximize loops - Will likely need to re-use graph.c

    // TODO later additions
    // - Display menu in visual graph Ex.
    // 
    //   +-A--+                                              
    //   4    |                                            
    //   +-B  6
    //        |                                                                                            
    //     C+-+                                              
    //      |
    //      3
    //      |                                             
    //     D+
    //
    //
    //      B    C    D -------\
    //       \   |   /          \
    //        2  3  4            12
    //         \ | /              \
    //  E --5--- A ---6-- F --9--- K
    //         / | \
    //        7  8  9
    //       /   |   \
    //      G    H    I
    //
    //
    //
    // - Add command option to override relative position of ciphertext and crib
    // - Support for menus > 12 characters


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
    // TODO allow override of starting rotorModels (will have to keep track of tested combinations).
    int rotorModels[3] = {0, 1, 2};
    
    // Initialize scrambler rotors and set relative position
    for (int i = 0; i < length; i++) {
        configureRotors(rotorModels, doubleEndedScrambler[i]);
        // TODO allow override of starting rotor positions (add logic to prevent accidental redoing of work).
        // TODO default starting position to (slow, middle, fast) Z, Z, Z + offset
        doubleEndedScrambler[i][FAST_ROTOR].position = i + 1;       // +1 offset as encodeChar does not step rotors.
    }

    // Determine additional connections for each letter in menu.
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

    // TODO Determine most common letter to connect to test register
    // - Merge sort Note: O(nlog(n)) time complexity may not be worth it for such a small dataset.
    // - Iterate through once counting and replacing letter depending on frequency.
    char testRegisterLetter = 'R';
    int testRegister = testRegisterLetter - 'A';

    // TODO Assume which letter the test register is steckered to
    // Note: do I need to ensure there is at least one adjacent vertex?
    char assumptionLetter = 'B';
    int assumptionLetterIndex = assumptionLetter - 'A';

    // Propagate testRegister signal through scrambler connected wires.
    traverseGraph(&valueMatrix[testRegister][assumptionLetterIndex]);

    printGraph(valueMatrix);

    // TODO evaluate matrix results and derive conclusions


    // Clear valueMatrix vertex value and adjacency list (retain diagonal board connections)
    for (int cable = 0; cable < ALPHA_LENGTH; cable++) {
        for (int wire = 0; wire < ALPHA_LENGTH; wire++) {
            clean(&valueMatrix[cable][wire].adj, cable != wire);
            valueMatrix[cable][wire].val = 0;
        }
    }

    return EXIT_SUCCESS;
}
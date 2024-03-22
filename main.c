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
    // Init vars.
    char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char cipher[CHAR_LIMIT]; char crib[CHAR_LIMIT];
    int flag;

    // TODO replace program name with argv[0]
    char programName[] = "!bombe";

    // Configure GetOpts
    int long_index = 0;
    static struct option long_options[] = {
        {"help",        no_argument,        0,  'h' },
        {0,             0,                  0,   0  }
    };

    // While there are arguments with options flags
    while ((flag = getopt_long(argc, argv, "h", long_options, &long_index)) != -1) {

        switch(flag) {
            case 'h': {
                printUsage(programName, false);
                exit(EXIT_SUCCESS);
            }
            default: errorExit(S_INVALID_FLAG, "main()", programName, true);
        }
    }

    // If there are non-flag arguments copy them to cipher and crib buffers
    if (optind + 1 < argc) {
        strncpy(cipher, argv[optind], CHAR_LIMIT);
        strncpy(crib, argv[optind + 1], CHAR_LIMIT);
        cipher[CHAR_LIMIT - 1] = '\0';
        crib[CHAR_LIMIT - 1] = '\0';
    }
    else {
        printf("ERROR: incorrect number of arguments specified. \n");
        printUsage(programName, true);
        exit(EXIT_FAILURE);
    }

    // Detect crib/cipher crashes
    int i = 0;
    while(cipher[i] != '\0' && crib[i] !='\0') {
        if(cipher[i] == crib[i]) return S_CRASH;
        i++;
    }
    // Determine crib/cipher length
    if(cipher[i] != '\0' || crib[i] !='\0') return S_INVALID_LENGTH;
    int length = i;

    // TODO: Add support for cipher/crib pairs less than 12 characters

    // TODO Determine best menu programatically
    // - Loop through each possible menu combination (pick m from n objects where order doesn't matter)
    // - Maximize loops - Will likely need to re-use graph.c
    //      - Allow GetOpt to halt testing once a menu with X cycles is found.

    // TODO create menu generation function
    // ------------------------------------------------ test
    configureMenu(cipher, crib, length);
    exit(1);
    // ------------------------------------------------ test


    // Build a graph and calculate the # of cycles
    // Choose the menu with most loops (or auto select one that reaches a certain threshold such as 3 loops)


    // Recursive function 
    
    





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
    // - Add command option to override relative position of cipher and crib
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
        int cipherCharIndex = cipher[i] - 'A';
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
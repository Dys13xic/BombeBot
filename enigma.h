#ifndef ENIGMA_H
#define ENIGMA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define FAST_ROTOR 2
#define MIDDLE_ROTOR 1
#define SLOW_ROTOR 0

typedef struct rotor {
    char cipherAlphabet[27];
    int position;
    int turnover;
} ROTOR;

int moduloSubtract(int x, int y, int mod);

char *convertMessage(ROTOR rotorSet[], char *plugboardCipher, char *message);

#endif
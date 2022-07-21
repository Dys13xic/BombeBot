#ifndef ENIGMA_H
#define ENIGMA_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#define FAST_ROTOR 2
#define MIDDLE_ROTOR 1
#define SLOW_ROTOR 0

#define BATTERY_SCRAMBLER_AMOUNT 12

typedef struct rotor {
    char cipherAlphabet[27];
    int position;
    int turnover;
} ROTOR;

typedef struct wire {
    struct wire *input;
    struct wire *output[BATTERY_SCRAMBLER_AMOUNT + 1];
    bool current;
} WIRE;


int moduloSubtract(int x, int y, int mod);

char *convertMessage(ROTOR rotorSet[], char *plugboardCipher, char *message);

#endif
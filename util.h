#include <stdlib.h>
#include <stdbool.h>
#include "stack.h"

typedef struct Chip8 {
    int8_t mem[4096];
    bool display[64*32];

    Stack* stk;
    int8_t* pc;
    int8_t i;

    int8_t delayTimer;
    int8_t soundTimer;
    int8_t v[16];
} Chip8;

int16_t fetch(Chip8*);
void decode_execute(int16_t, Chip8*);

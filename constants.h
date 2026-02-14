#include <stdlib.h>
#include "stack.h"

#define LIGHT_COLOR 0xFF000000
#define DARK_COLOR 0xFFFFFFFF

#define WIDTH 64
#define HEIGHT 32

typedef struct Chip8 {
    uint8_t mem[4096];
    bool display[64*32];

    Stack* stk;
    uint8_t* pc;
    uint8_t* i;

    uint8_t delayTimer;
    uint8_t soundTimer;
    uint8_t v[16];
} Chip8;

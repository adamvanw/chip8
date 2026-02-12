#include <SDL3/SDL.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"

int main(int argc, char** argv) {
    Chip8* c8 = malloc(sizeof(Chip8));

    decode_execute(fetch(c8), c8);
}

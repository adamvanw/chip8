#include <stdbool.h>
#include <SDL3/SDL_iostream.h>
#include "init.h"


void init_chip8(Chip8* c8) {
    c8->pc = &c8->mem[0x0200];

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            c8->display[x + y * WIDTH] = false;
        }
    }
}

bool populate_memory(Chip8* c8, char* filename) {
    printf("%s\n", filename);
    SDL_IOStream* stream = SDL_IOFromFile(filename, "r");
    if (stream == NULL) return false;

    uint16_t pos = 0x0200;
    while (SDL_ReadIO(stream, &c8->mem[pos++], 1) != 0) {

    }

    SDL_CloseIO(stream);
    return true;
}

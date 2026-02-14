#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include "init.h"

uint16_t fetch(Chip8*);
void decode_execute(uint16_t, Chip8*);

void get_inputs();
void update_screen(Chip8*, SDL_Surface*);

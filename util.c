#include "util.h"
#include "SDL3/SDL_render.h"

void update_screen(Chip8* c8, SDL_Surface* screen) {
    SDL_LockSurface(screen);
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x ++) {
            uint32_t* ptr = (uint32_t*)screen->pixels + (y * screen->pitch/4 + x);
            *ptr = (c8->display[x + y * WIDTH]) ? DARK_COLOR : LIGHT_COLOR;
        }
    }
    SDL_UnlockSurface(screen);
}

uint16_t fetch(Chip8* c8) {
    uint16_t val = ((uint16_t)(*c8->pc) << 8) + *(c8->pc + 1);
    c8->pc = c8->pc + 2;
    printf("Address: %p\n", c8->pc);
    return val;
};

void decode_execute(uint16_t val, Chip8* c8) {
    printf("Val: %d\n", val);
    printf("First num: %d\n", val >> 12);
    uint16_t nnn = val % 4096;
    uint8_t nn = val % 256;
    uint8_t n = val % 16;
    uint8_t x = (val >> 8) % 16;
    uint8_t y = (val >> 4) % 16;
    switch (val >> 12) {
        case 0: // clear screen (00E0)
            printf("Clearing screen.\n");
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    c8->display[j + i * HEIGHT] = false;
                }
            }

            printf("Cleared screen.\n");
            break;
        case 1: // jump (1NNN)
            printf("Jumping.\n");
            c8->pc = &c8->mem[nnn];
            printf("Jumped.\n");
            break;
        case 6: // set register (6XNN)
            printf("Setting register.\n");
            c8->v[x] = nn;
            printf("Set register.\n");
            break;
        case 7: // add val to register (7XNN)
            printf("Adding val to register.\n");
            c8->v[x] += nn;
            printf("Added val to register.\n");
            break;
        case 10: // set index register (ANNN)
            printf("Setting index register.\n");
            c8->i = &c8->mem[nnn];
            printf("Set index register.\n");
            break;
        case 13: // display (DXYN)
            printf("Displaying.\n");
            int y_in = c8->v[y];

            c8->v[15] = 0;
            for (int i = 0; i < n; i++)  {
                uint8_t row = *(c8->i + i);
                int x_in = c8->v[x];
                for (int bit = 7; bit >= 0; bit--) {
                    int flag = (row >> bit) % 2;
                    bool pixel = c8->display[y_in * WIDTH + x_in];

                    if (flag == 1 && pixel) {
                        c8->display[y_in * WIDTH + x_in] = false;
                        c8->v[15] = 1;
                        printf("Setting pixel to false.\n");
                    } else if (flag == 1 && !pixel) {
                        c8->display[y_in * WIDTH + x_in] = true;
                        printf("Setting pixel to true.\n");
                    }
                    if (bit + x_in >= WIDTH) bit = -1;
                    ++x_in;
                }
                ++y_in;
                if (y_in >= HEIGHT) break;
            }

            printf("Updated display.\n");
            break;
        default:
            break;
    }
}

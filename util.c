#include "util.h"

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

    uint16_t sum;
    switch (val >> 12) {
        // Display logic
        case 0: // clear screen (00E0)
            printf("Clearing screen.\n");
            for (int i = 0; i < HEIGHT; i++) {
                for (int j = 0; j < WIDTH; j++) {
                    c8->display[j + i * HEIGHT] = false;
                }
            }

            printf("Cleared screen.\n");
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

        // Jumps and conditional jumps
        case 1: // jump (1NNN)
            printf("Jumping.\n");
            c8->pc = &c8->mem[nnn];
            printf("Jumped.\n");
            break;
        case 3: // jump if VX == NN (3XNN)
            if (c8->v[x] == nn) c8->pc += 2;
            break;
        case 4: // jump if VX != NN (4XNN)
            if (c8->v[x] != nn) c8->pc += 2;
            break;
        case 5: // jump if VX == VY (5XY0)
            if (c8->v[x] == c8->v[y]) c8->pc += 2;
            break;
        case 9: // jump if VX != VY (5XY0)
            if (c8->v[x] != c8->v[y]) c8->pc += 2;
            break;

        // Logical instructions
        case 8:
            switch (n) {
                case 0:
                    c8->v[x] = c8->v[y];
                    break;
                case 1:
                    c8->v[x] = c8->v[x] | c8->v[y];
                    break;
                case 2:
                    c8->v[x] = c8->v[x] & c8->v[y];
                    break;
                case 3:
                    c8->v[x] = c8->v[x] ^ c8->v[y];
                    break;
                case 4:
                    sum = c8->v[x] + c8->v[y];
                    c8->v[x] = c8->v[x] + c8->v[y];
                    if (sum > c8->v[x]) c8->v[15] = 1;
                    else c8->v[15] = 0;
                    break;
                case 5:
                    if (c8->v[x] > c8->v[y]) c8->v[15] = 1;
                    else c8->v[15] = 0;

                    c8->v[x] = c8->v[x] - c8->v[y];
                    break;
                case 7:
                    if (c8->v[y] > c8->v[x]) c8->v[15] = 1;
                    else c8->v[15] = 0;

                    c8->v[x] = c8->v[y] - c8->v[x];
                    break;

                case 6:
                    // c8->v[x] = c8->v[y];

                    bool bit = c8->v[x] % 2 == 1;
                    c8->v[x] >>= 1;

                    if (bit) c8->v[15] = 1;
                    else c8->v[15] = 0;
                    break;

                case 14:
                    // c8->v[x] = c8->v[y];

                    bit = c8->v[x] >= 128;
                    c8->v[x] <<= 1;

                    if (bit) c8->v[15] = 1;
                    else c8->v[15] = 0;

                    break;
                default:
                    break;
            }
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

        case 15:
            switch (nn) {
                case 7:
                    c8->v[x] = c8->delayTimer;
                    break;
                case 21:
                    c8->delayTimer = c8->v[x];
                    break;
                case 24:
                    c8->soundTimer = c8->v[x];
                    break;
                case 31:
                    // Since our i register is uint8_t*, we need
                    // to get c8->i and c8->i + 1.
                    sum = (*c8->i << 4) + (*c8->i + 1) + c8->v[x];
                    if (sum >= 0x1000) c8->v[0xF] = 1;
                    *c8->i = sum >> 8;
                    *(c8->i + 1) = sum % 256;
                    break;
            }
        default:
            break;
    }
}

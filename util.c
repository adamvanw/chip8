#include "util.h"

int16_t fetch(Chip8* c8) {
    int16_t val = ((int16_t)(*c8->pc) << 8) + *(c8->pc + 1);
    c8->pc = c8->pc + 2;
    return val;
};

void decode_execute(int16_t val, Chip8* c8) {
    switch (val >> 12) {
        case 0: // clear screen (00E0)
            for (int i = 0; i < 32; i++) {
                for (int j = 0; j < 64; j++) {
                    c8->display[j + i * 32] = false;
                }
            }
            break;
        case 1: // jump (1NNN)
            c8->pc = &c8->mem[val % 4096];
            break;
        case 6: // set register (6XNN)
            c8->v[(val << 4) >> 12] = val % 256;
            break;
        case 7: // add val to register (7XNN)
            c8->v[(val << 4) >> 12] += val % 256;
            break;
        case 10: // set index register (ANNN)
            c8->i = (val << 4) >> 4;
            break;
        case 13: // display (DXYN)
            int n = val % 16;
            int x = c8->v[(val << 4) >> 12] % 64;
            int x_og = x;
            int y = c8->v[(val << 8) >> 12] % 32;
            c8->v[15] = 0;
            for (int i = 0; i < n; i++)  {
                int8_t row = c8->mem[c8->i + n];
                int x = x_og;
                for (int bit = 7; bit >= 0; bit--) {
                    int flag = (row >> bit) % 2;
                    int8_t pixel = c8->display[(row + y) * 32 + x];
                    if (flag && pixel) {
                        c8->display[(row + y) * 32 + x] = false;
                        c8->v[15] = 1;
                    } else if (flag && !pixel) {
                        c8->display[(row + y) * 32 + x] = true;
                    }
                    if (bit + x >= 64) bit = -1;
                    ++x;
                }
                ++y;
                if (y + row >= 32) break;
            }
            break;
        default:
            break;
    }
}

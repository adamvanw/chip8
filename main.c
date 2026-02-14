#include <stdio.h>
#include <string.h>

#include "SDL3/SDL_error.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "util.h"

int main(int argc, char** argv) {
    if (argc <= 1) {
        printf("Please provide a file! (*.c8)\n");
        return 1;
    }

    Chip8* c8 = malloc(sizeof(Chip8));

    if (!populate_memory(c8, argv[1])) {
        printf("Memory was not populated. Error: %s\n", SDL_GetError());
        return 1;
    }
    init_chip8(c8);

    bool sdl_started = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS);
    if (!sdl_started) {
        printf("SDL failed to start: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Chip8 Emu", WIDTH * 12, HEIGHT * 12, SDL_WINDOW_RESIZABLE);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, "gpu");
    SDL_Surface* screen = SDL_CreateSurface(WIDTH, HEIGHT, SDL_PIXELFORMAT_RGBA32);
    SDL_FillSurfaceRect(screen, NULL, 0x000000FF);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, screen);
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);
    SDL_FRect srcRect = {0, 0, WIDTH, HEIGHT};
    SDL_FRect dstRect = {0, 0, WIDTH * 12, HEIGHT * 12};

    bool quit = false;
    SDL_Event event;
    while (!quit) {
        decode_execute(fetch(c8), c8);
        update_screen(c8, screen);

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.key == SDLK_ESCAPE) {
                        quit = true;
                    }
                    break;
            }
        }

        SDL_DestroyTexture(texture);
        texture = SDL_CreateTextureFromSurface(renderer, screen);
        SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_PIXELART);

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, &srcRect, &dstRect);
        SDL_RenderPresent(renderer);

        SDL_Delay(90);
    }
}

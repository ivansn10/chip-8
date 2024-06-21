#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"

void draw_screen(System *system, SDL_Renderer *renderer) {
    for(int x = 0; x < DISPLAY_X; x++) {
        for(int y = 0; y < DISPLAY_Y; y++) {
            if (system->display[x][y] == 1) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black for '1'
            } else {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White for '0' or any other value
            }
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}

void run_emulator(ROM *rom)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow(rom->path, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, DISPLAY_X, DISPLAY_Y, SDL_WINDOW_OPENGL);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    int quit = 0;

    System system = {0};

    while (!quit)
    {
        while(SDL_PollEvent(&event) != 0) {
            if(event.type == SDL_QUIT) {
                quit = 1;
            }
        }
        //process(&system, rom);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);
        draw_screen(&system, renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    window = NULL;
    renderer = NULL;

    SDL_Quit();
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("The format is not correct.\n");
        return 1;
    }

    ROM rom = {argv[1]};
    run_emulator(&rom);
    return 0;
}
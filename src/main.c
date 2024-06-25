#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"

void draw_screen(System *system, SDL_Renderer *renderer)
{
    for (int x = 0; x < DISPLAY_X; x++)
    {
        for (int y = 0; y < DISPLAY_Y; y++)
        {
            if (system->display[x][y] == 1)
            {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
    initialize(rom, &system);

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            // handle_input(&event);
        }
        process(&system, rom);
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

void load_rom(ROM *rom)
{
    if (!rom || !rom->path)
    {
        fprintf(stderr, "Cannot read ROM.\n");
        return;
    }

    FILE *file = fopen(rom->path, "r");
    if (!file)
    {
        perror("Failed to open file\n");
        return;
    }

    size_t bytesRead = fread(rom->data, sizeof(unsigned char), sizeof(rom->data), file);
    if (bytesRead < sizeof(rom->data))
    {
        if (ferror(file))
        {
            perror("Error reading file\n");
        }
    }

    fclose(file);
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("The format is not correct.\n");
        return 1;
    }

    ROM rom = {0};
    rom.path = argv[1];
    load_rom(&rom);
    run_emulator(&rom);
    return 0;
}
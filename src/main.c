#include <stdio.h>
#include <SDL2/SDL.h>
#include "cpu.h"
#include "input.h"

void draw_screen(System *system, SDL_Renderer *renderer)
{
    //Calculate the scale factor
    float scaleX = (float)system->windowX / DISPLAY_X;
    float scaleY = (float)system->windowY / DISPLAY_Y;
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
            //Draw a rectangle with scaling for this point
            SDL_Rect rect;
            rect.x = x * scaleX;
            rect.y = y * scaleY;
            rect.w = scaleX;
            rect.h = scaleY;

            SDL_RenderFillRect(renderer, &rect);
        }
    }
}

void run_emulator(ROM *rom)
{
    System system = {0};
    initialize(rom, &system);
    struct KeypadPair keyTable[16] = {
        {SDL_SCANCODE_0, 0x0,},
        {SDL_SCANCODE_1, 0x1,},
        {SDL_SCANCODE_2, 0x2,},
        {SDL_SCANCODE_3, 0x3,},
        {SDL_SCANCODE_4, 0x4,},
        {SDL_SCANCODE_5, 0x5,},
        {SDL_SCANCODE_6, 0x6,},
        {SDL_SCANCODE_7, 0x7,},
        {SDL_SCANCODE_8, 0x8,},
        {SDL_SCANCODE_9, 0x9,},
        {SDL_SCANCODE_A, 0xA,},
        {SDL_SCANCODE_B, 0xB,},
        {SDL_SCANCODE_C, 0xC,},
        {SDL_SCANCODE_D, 0xD,},
        {SDL_SCANCODE_E, 0xE,},
        {SDL_SCANCODE_F, 0xF,}
    };
    
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, system.windowX, system.windowY, SDL_WINDOW_OPENGL);
    SDL_SetWindowMaximumSize(window, system.windowX, system.windowY);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Event event;
    int quit = 0;

    while (!quit)
    {
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                quit = 1;
            }
            handle_input(&event, &system, keyTable);
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
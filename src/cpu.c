#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include "cpu.h"

void fetch(System *system) {
    
}

void decode(System *system) {

}

void execute(System *system) {

}

void process(System *system, ROM *rom) {
    static double lastFrameTime = 0.0;
    static double accumulatedTime = 0.0;
    float deltaTime = (SDL_GetTicks64() - lastFrameTime) / 1000.0f;
    accumulatedTime += deltaTime;

    const int targetIPS = 500;
    const float targetFrameRate = 60;
    const float secondsPerFrame = 1.0 / 60;

    int frameInstructions = (int)(targetIPS * accumulatedTime);

    double startFrameTime = SDL_GetTicks64();

    for(int i = 0; i < frameInstructions; i++) {
        fetch(system);
        decode(system);
        execute(system);
    }

    accumulatedTime -= frameInstructions / (float)targetIPS;
    double processedFrameTime = SDL_GetTicks64() - startFrameTime;

    lastFrameTime = SDL_GetTicks64();
}
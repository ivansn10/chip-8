#include <raylib.h>
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
    static double accumulatedTime = 0.0;
    int instructionCount = 0;
    float frameTime = GetFrameTime();
    accumulatedTime += frameTime;

    const int targetIPS = 500;
    const float targetFrameRate = 60;
    const float secondsPerFrame = 1.0 / 60;

    int frameInstructions = (int)(targetIPS * accumulatedTime);

    double startFrameTime = GetTime();

    for(int i = 0; i < frameInstructions; i++) {
        fetch(system);
        decode(system);
        execute(system);
        instructionCount++;
    }

    accumulatedTime -= frameInstructions / (float)targetIPS;
    double processedFrameTime = GetTime() - startFrameTime;
}
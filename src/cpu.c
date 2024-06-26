#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cpu.h"
#include "instructions.h"

struct InstructionDecode {
    unsigned short opcode;
    void (*instruction)(unsigned short, System*);
    unsigned short mask;
};

void initialize(ROM *rom, System *system) {
    system->programCounter = 0x200;
    system->stackPointer = 0;

    unsigned const char fontset[80] =
    { 
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    for(int i = 0; i < 80; i++) {
        system->fontset[i] = fontset[i];
    }

    //load fontset into memory at 0x050
    for(int i = 0; i < 80; i++) {
        system->memory[0x050 + i] = fontset[i];
    }

    //Load ROM into memory at 0x200
    for(int i = 0; i < 3584; i++) {
        system->memory[0x200 + i] = rom->data[i];
    }

    //Initialize the RNG
    srand(time(NULL));

    //Set the window size
    system->windowX = 1280;
    system->windowY = 640;
}

void execute(void (*instruction)(unsigned short, System*), unsigned short opcode, System *system) {
    instruction(opcode, system);
}

void decode(unsigned short opcode, System *system) {

    const struct InstructionDecode instructionTable[35] = {
        {0x00E0, execute00E0, 0xFFFF},
        {0x00EE, execute00EE, 0xFFFF},
        {0x0000, execute0NNN, 0xF000},
        {0x1000, execute1NNN, 0xF000},
        {0x2000, execute2NNN, 0xF000},
        {0x3000, execute3XKK, 0xF000},
        {0x4000, execute4XKK, 0xF000},
        {0x5000, execute5XY0, 0xF00F},
        {0x6000, execute6XKK, 0xF000},
        {0x7000, execute7XKK, 0xF000},
        {0x8000, execute8XY0, 0xF00F},
        {0x8001, execute8XY1, 0xF00F},
        {0x8002, execute8XY2, 0xF00F},
        {0x8003, execute8XY3, 0xF00F},
        {0x8004, execute8XY4, 0xF00F},
        {0x8005, execute8XY5, 0xF00F},
        {0x8006, execute8XY6, 0xF00F},
        {0x8007, execute8XY7, 0xF00F},
        {0x800E, execute8XYE, 0xF00F},
        {0x9000, execute9XY0, 0xF00F},
        {0xA000, executeANNN, 0xF000},
        {0xB000, executeBNNN, 0xF000},
        {0xC000, executeCXKK, 0xF000},
        {0xD000, executeDXYN, 0xF000},
        {0xE09E, executeEX9E, 0xF0FF},
        {0xE0A1, executeEXA1, 0xF0FF},
        {0xF007, executeFX07, 0xF0FF},
        {0xF00A, executeFX0A, 0xF0FF},
        {0xF015, executeFX15, 0xF0FF},
        {0xF018, executeFX18, 0xF0FF},
        {0xF01E, executeFX1E, 0xF0FF},
        {0xF029, executeFX29, 0xF0FF},
        {0xF033, executeFX33, 0xF0FF},
        {0xF055, executeFX55, 0xF0FF},
        {0xF065, executeFX65, 0xF0FF}
    };

    for(int i = 0; i < 35; i++) {
        if((opcode & instructionTable[i].mask) == instructionTable[i].opcode) {
            //printf("%#04x : The opcode is %#04x and the mask is %#04x and the applied mask is %#04x\n",system->programCounter, opcode, instructionTable[i].mask, opcode & instructionTable[i].mask);
            execute(instructionTable[i].instruction, opcode, system);
            return;
        }
    }
}

void fetch(System *system) {
    //fetch the instruction from RAM using the programCounter
    unsigned short opcode = (system->memory[system->programCounter] << 8) | (system->memory[system->programCounter + 1]);
    decode(opcode, system);
}

void process(System *system, ROM *rom) {
    static double endFrameTime = 0.0;
    static double accumulatedTime = 0.0;
    float deltaTime = (SDL_GetTicks64() - endFrameTime) / 1000.0f;
    accumulatedTime += deltaTime;

    const int targetIPS = 500;
    const float targetFrameRate = 60;

    int frameInstructions = (int)(targetIPS * accumulatedTime);

    double startFrameTime = SDL_GetTicks64();

    for(int i = 0; i < frameInstructions; i++) {
        fetch(system);
    }

    accumulatedTime -= frameInstructions / (float)targetIPS;

    if(system->delayTimer > 0) {
        //printf("delay timer is %d\n", system->delayTimer);
        system->delayTimer -= 1;
    }

    if(system->soundTimer > 0) {
        system->soundTimer -= 1;
    }
    endFrameTime = SDL_GetTicks64();
}
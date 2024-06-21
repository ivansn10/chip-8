#ifndef CPU_H
#define CPU_H

#define DISPLAY_X 64 
#define DISPLAY_Y 32
#define REFRESH_RATE 60
#define TIMER_RATE 60
#define CLOCK_SPEED 500

typedef struct System {
    //CPU
    unsigned char programCounter[2];
    unsigned char stack[16];
    unsigned char stackPointer[2];
    unsigned char dataRegisters[16]; //V0-VF
    unsigned char addressRegister[2]; //I
    unsigned char delayTimer;
    unsigned char soundTimer;

    //RAM
    unsigned char memory[4096]; //0x200

    //Display
    unsigned char display[64][32];

    //Input
    unsigned char keys[2]; //0-F keys
} System;

typedef struct ROM
{
    const char *path;
}ROM;

void process(System *system, ROM *rom);

#endif
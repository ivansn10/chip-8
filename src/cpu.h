#ifndef CPU_H
#define CPU_H

#define DISPLAY_X 64 
#define DISPLAY_Y 32
#define REFRESH_RATE 60
#define TIMER_RATE 60
#define CLOCK_SPEED 500
#define RAM_SIZE 4096

typedef struct System {
    //CPU
    unsigned short programCounter;
    unsigned char stack[16];
    unsigned short stackPointer;
    unsigned char dataRegisters[16]; //V0-VF
    unsigned short addressRegister; //I
    unsigned char delayTimer;
    unsigned char soundTimer;

    //RAM
    unsigned char memory[RAM_SIZE]; //0x200

    //Fontset
    unsigned char fontset[80];

    //Display
    unsigned char display[64][32];

    //Input
    unsigned short keys; //0-F keys
} System;

typedef struct ROM
{
    unsigned char data[3584];
    const char *path;
}ROM;

void initialize(ROM *rom, System* system);
void process(System *system, ROM *rom);

#endif
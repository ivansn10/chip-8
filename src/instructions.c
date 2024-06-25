#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>


void execute0NNN(unsigned short opcode, System *system) {
    //SYS addr, ignored
    printf("You are executing 0NNN\n");
}

void execute00E0(unsigned short opcode, System *system) {
    //CLS - clear screen
    printf("You are executing 00E0\n");
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 32; y++) {
            system->display[x][y] = 0;
        }
    }
}
void execute00EE(unsigned short opcode, System *system) {
    //RET - Return from a subroutine
    printf("You are executing 00EE\n");
    system->stackPointer--;
    system->programCounter = system->stack[system->stackPointer];
}
void execute1NNN(unsigned short opcode, System *system) {
    //JP addr
    printf("You are executing 1NNN\n");
    unsigned short addr = opcode & 0x0FFF;
    system->programCounter = addr;
}

void execute2NNN(unsigned short opcode, System *system) {
    //CALL addr
    printf("You are executing 2NNN\n");
    if(system-> stackPointer < 8) {
        system->stack[system->stackPointer] = system->programCounter;
        system->stackPointer++;
        system->programCounter = opcode & 0x0FFF;
    }    
}

void execute3XKK(unsigned short opcode, System *system) {
    //SE Vx, byte 
    printf("You are executing 3XKK\n");
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] == opcode & 0x00FF) {
        system->programCounter+= 2;
    }
}

void execute4XKK(unsigned short opcode, System *system) {
    //SNE Vx, byte
    printf("You are executing 4XKK\n");
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] != opcode & 0x00FF) {
        system->programCounter+= 2;
    }
}

void execute5XY0(unsigned short opcode, System *system) {
    //SE Vx, Vy
    printf("You are executing 5XY0\n");
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] == system->dataRegisters[(opcode & 0x00F0) >> 4]) {
        system->programCounter+= 2;
    }
}

void execute6XKK(unsigned short opcode, System *system) {
    //LD Vx, byte
    printf("You are executing 6XKK\n");
    unsigned char v = (opcode & 0x0F00) >> 8;
    unsigned char b = opcode & 0x00FF;
    system->dataRegisters[v] = b;
}

void execute7XKK(unsigned short opcode, System *system) {
    //ADD Vx, byte
    printf("You are executing 7XKK\n");
    unsigned char v = (opcode & 0x0F00) >> 8;
    unsigned char b = opcode & 0x00FF;
    system->dataRegisters[v] += b; 
}

void execute8XY0(unsigned short opcode, System *system) {
    //LD Vx, Vy
    printf("You are executing 8XY0\n");
    system->dataRegisters[(opcode & 0x0F00) >> 8] = system->dataRegisters[(opcode & 0x00F0) >> 4];
}

void execute8XY1(unsigned short opcode, System *system) {
    //OR Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] | system->dataRegisters[vy];
}

void execute8XY2(unsigned short opcode, System *system) {
    //AND Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] & system->dataRegisters[vy];
}

void execute8XY3(unsigned short opcode, System *system) {
    //XOR Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] ^ system->dataRegisters[vy];
}

void execute8XY4(unsigned short opcode, System *system) {
    //ADD Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    unsigned short result = system->dataRegisters[vx] + system->dataRegisters[vy];

    system->dataRegisters[0xF] = (result > 0xFF) ? 1 : 0;

    system->dataRegisters[vx] = result & 0xFF;
}

void execute8XY5(unsigned short opcode, System *system) {
    //SUB Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    unsigned char result = system->dataRegisters[vx] - system->dataRegisters[vy];

    system->dataRegisters[0xF] = (system->dataRegisters[vx] > system->dataRegisters[vy]) ? 1 : 0;

    system->dataRegisters[vx] = result;
}

void execute8XY6(unsigned short opcode, System *system) {
    //SHR Vx {, Vy}
}

void execute8XY7(unsigned short opcode, System *system) {
    //SUBN Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    unsigned char result = system->dataRegisters[vy] - system->dataRegisters[vx];

    system->dataRegisters[0xF] = (system->dataRegisters[vy] > system->dataRegisters[vx]) ? 1 : 0;

    system->dataRegisters[vx] = result;
}

void execute8XYE(unsigned short opcode, System *system) {
    //SHL Vx {, Vy}
}

void execute9XY0(unsigned short opcode, System *system) {
    //SNE Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    if(system->dataRegisters[vx] != system->dataRegisters[vy]) system->programCounter += 2;
}

void executeANNN(unsigned short opcode, System *system) {
    //LD I, addr
    printf("You are executing ANNN\n");
    unsigned short addr = opcode & 0x0FFF;
    system->addressRegister = addr;
}

void executeBNNN(unsigned short opcode, System *system) {
    //JP V0, addr
    printf("You are executing BNNN\n");
    system->programCounter = (opcode & 0x0FFF) + system->dataRegisters[0];
}

void executeCXKK(unsigned short opcode, System *system) {
    //RND Vx, byte
    printf("You are executing CXKK\n");
    unsigned char randomByte = rand() % 256;
    unsigned char kk = opcode & 0x00FF;
    system->dataRegisters[(opcode & 0x0F00) >> 8] = randomByte & kk;
}

void executeDXYN(unsigned short opcode, System *system) {
    //DRW Vx, Vy, nibble
    printf("You are executing DXYN\n");
    unsigned short x = (opcode & 0x0F00) >> 8;
    unsigned short y = (opcode & 0x00F0) >> 4;
    unsigned short height = opcode & 0x000F; //rows

    //wrap the starting position using modulo
    unsigned short xPosition = system->dataRegisters[x] % DISPLAY_X;
    unsigned short yPosition = system->dataRegisters[y] % DISPLAY_Y;

    system->dataRegisters[0xF] = 0; // Reset VF before drawing for possible collision detection

    for(int row = 0; row < height; row++) {
        unsigned char byte = system->memory[system->addressRegister + row];

        for(int col = 0; col < 8; col++) {
 
            //we check if our desired position is inside the display range
            if ((byte & (0x80 >> col)) != 0) { // Check each bit in the sprite byte, the 0x80 mask checks for each bit
                unsigned int x = (xPosition + col) % DISPLAY_X;
                unsigned int y = (yPosition + row) % DISPLAY_Y;
                if (system->display[x][y] == 1) {
                    system->dataRegisters[0xF] = 1; // Set VF to 1 if collision occurs
                }
                system->display[x][y] ^= 1; // XOR to draw/erase the pixel
            }
        }
    }
}

void executeEX9E(unsigned short opcode, System *system) {
    //SKP Vx
}

void executeEXA1(unsigned short opcode, System *system) {
    //SKNP Vx
}

void executeFX07(unsigned short opcode, System *system) {
    //LD Vx, DT
}

void executeFX0A(unsigned short opcode, System *system) {
    //LD Vx, K
    
}

void executeFX15(unsigned short opcode, System *system) {
    //LD DT, Vx
}

void executeFX18(unsigned short opcode, System *system) {
    //LD ST, Vx
}

void executeFX1E(unsigned short opcode, System *system) {
    //ADD I, Vx
    system->addressRegister = system->addressRegister + system->dataRegisters[(opcode & 0x0F00) >> 8];
}

void executeFX29(unsigned short opcode, System *system) {
    //LD F, Vx
}

void executeFX33(unsigned short opcode, System *system) {
    //LD B, Vx
}

void executeFX55(unsigned short opcode, System *system) {
    //LD [I], Vx
    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        system->memory[system->addressRegister + i] = system->dataRegisters[i];
    }
}

void executeFX65(unsigned short opcode, System *system) {
    //LD Vx, [I]
    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        system->dataRegisters[i] = system->memory[system->addressRegister + i];
    }
}
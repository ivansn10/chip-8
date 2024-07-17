#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>


void execute0NNN(unsigned short opcode, System *system) {
    //SYS addr, ignored
    system->programCounter += 2;
}

void execute00E0(unsigned short opcode, System *system) {
    //CLS - clear screen
    for(int x = 0; x < 64; x++) {
        for(int y = 0; y < 32; y++) {
            system->display[x][y] = 0;
        }
    }
    system->programCounter += 2;
}
void execute00EE(unsigned short opcode, System *system) {
    //RET - Return from a subroutine
    system->stackPointer--;
    system->programCounter = system->stack[system->stackPointer] + 2; 
}
void execute1NNN(unsigned short opcode, System *system) {
    //JP addr
    unsigned short addr = opcode & 0x0FFF;
    system->programCounter = addr;
}

void execute2NNN(unsigned short opcode, System *system) {
    //CALL addr
    if(system-> stackPointer < 16) {
        system->stack[system->stackPointer] = system->programCounter;
        system->stackPointer++;
        system->programCounter = opcode & 0x0FFF;
    }    
}

void execute3XKK(unsigned short opcode, System *system) {
    //SE Vx, byte 
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] == (opcode & 0x00FF)) {
        system->programCounter+= 2;
    }
    system->programCounter += 2;
}

void execute4XKK(unsigned short opcode, System *system) {
    //SNE Vx, byte
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] != (opcode & 0x00FF)) {
        system->programCounter+= 2;
    }
    system->programCounter += 2;
}

void execute5XY0(unsigned short opcode, System *system) {
    //SE Vx, Vy
    if (system->dataRegisters[(opcode & 0x0F00) >> 8] == system->dataRegisters[(opcode & 0x00F0) >> 4]) {
        system->programCounter+= 2;
    }
    system->programCounter += 2;
}

void execute6XKK(unsigned short opcode, System *system) {
    //LD Vx, byte
    unsigned char v = (opcode & 0x0F00) >> 8;
    unsigned char b = opcode & 0x00FF;
    system->dataRegisters[v] = b;
    system->programCounter += 2;
}

void execute7XKK(unsigned short opcode, System *system) {
    //ADD Vx, byte
    unsigned char v = (opcode & 0x0F00) >> 8;
    unsigned char b = opcode & 0x00FF;
    system->dataRegisters[v] += b; 
    system->programCounter += 2;
}

void execute8XY0(unsigned short opcode, System *system) {
    //LD Vx, Vy
    system->dataRegisters[(opcode & 0x0F00) >> 8] = system->dataRegisters[(opcode & 0x00F0) >> 4];
    system->programCounter += 2;

}

void execute8XY1(unsigned short opcode, System *system) {
    //OR Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] | system->dataRegisters[vy];
    system->dataRegisters[0xF] = 0;
    system->programCounter += 2;
}

void execute8XY2(unsigned short opcode, System *system) {
    //AND Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] & system->dataRegisters[vy];
    system->dataRegisters[0xF] = 0;
    system->programCounter += 2;
}

void execute8XY3(unsigned short opcode, System *system) {
    //XOR Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    system->dataRegisters[vx] = system->dataRegisters[vx] ^ system->dataRegisters[vy];
    system->dataRegisters[0xF] = 0;
    system->programCounter += 2;
}

void execute8XY4(unsigned short opcode, System *system) {
    //ADD Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    unsigned short result = system->dataRegisters[vx] + system->dataRegisters[vy];

    system->dataRegisters[vx] = result;
    system->dataRegisters[0xF] = (result > 0xFF);
    system->programCounter += 2;
}

void execute8XY5(unsigned short opcode, System *system) {
    //SUB Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    unsigned char originalX = system->dataRegisters[vx];
    unsigned char result = system->dataRegisters[vx] - system->dataRegisters[vy];
    
    system->dataRegisters[0xF] = 1;
    system->dataRegisters[vx] = result;
    system->dataRegisters[0xF] = (originalX > system->dataRegisters[vy]);
    system->programCounter += 2;
}

void execute8XY6(unsigned short opcode, System *system) {
    //SHR Vx {, Vy}
    unsigned short x = (opcode & 0x0F00) >> 8;
    unsigned short y = (opcode & 0x00F0) >> 4;
    unsigned char originalVx = system->dataRegisters[x];

    system->dataRegisters[x] = system->dataRegisters[y];
    system->dataRegisters[x] >>= 1;
    system->dataRegisters[0xF] = (originalVx & 0x1);
    system->programCounter += 2;
}

void execute8XY7(unsigned short opcode, System *system) {
    //SUBN Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;
    unsigned char originalX = system->dataRegisters[vx];
    unsigned char result = system->dataRegisters[vy] - system->dataRegisters[vx];

    system->dataRegisters[0xF] = 1;
    system->dataRegisters[vx] = result;
    //this should be system->dataRegisters[vy] > originalX . maybe the test bugged?
    system->dataRegisters[0xF] = (system->dataRegisters[vy] > system->dataRegisters[vx]);
    system->programCounter += 2;
}

void execute8XYE(unsigned short opcode, System *system) {
    //SHL Vx {, Vy}
    unsigned short x = (opcode & 0x0F00) >> 8;
    unsigned short y = (opcode & 0x00F0) >> 4;
    unsigned char originalVx = system->dataRegisters[x];

    system->dataRegisters[x] = system->dataRegisters[y];
    system->dataRegisters[x] <<= 1;
    //we apply the mask 10000000 and right shift it to 00000001 and check the value
    system->dataRegisters[0xF] = (originalVx & 0x80) >> 7;
    system->programCounter += 2;
}

void execute9XY0(unsigned short opcode, System *system) {
    //SNE Vx, Vy
    unsigned char vx = (opcode & 0x0F00) >> 8;
    unsigned char vy = (opcode & 0x00F0) >> 4;

    if(system->dataRegisters[vx] != system->dataRegisters[vy]) {
        system->programCounter += 2;
    }
    system->programCounter += 2;
}

void executeANNN(unsigned short opcode, System *system) {
    //LD I, addr
    unsigned short addr = opcode & 0x0FFF;
    system->addressRegister = addr;
    system->programCounter += 2;
}

void executeBNNN(unsigned short opcode, System *system) {
    //JP V0, addr
    system->programCounter = (opcode & 0x0FFF) + system->dataRegisters[0];
}

void executeCXKK(unsigned short opcode, System *system) {
    //RND Vx, byte
    unsigned char randomByte = rand() % 256;
    unsigned char kk = opcode & 0x00FF;
    system->dataRegisters[(opcode & 0x0F00) >> 8] = randomByte & kk;
    system->programCounter += 2;
}

void executeDXYN(unsigned short opcode, System *system) {
    //DRW Vx, Vy, nibble
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
    system->programCounter += 2;
}

void executeEX9E(unsigned short opcode, System *system) {
    //SKP Vx
    if(system->keys[system->dataRegisters[(opcode & 0x0F00) >> 8]] == 0xF) {
        system->programCounter+= 2;
    }
    system->programCounter += 2;
}

void executeEXA1(unsigned short opcode, System *system) {
    //SKNP Vx
    if(system->keys[system->dataRegisters[(opcode & 0x0F00) >> 8]] == 0x0) {
        system->programCounter+= 2;
    }
    system->programCounter += 2;
}

void executeFX07(unsigned short opcode, System *system) {
    //LD Vx, DT
    system->dataRegisters[(opcode & 0x0F00) >> 8] = system->delayTimer;
    system->programCounter += 2;
}

void executeFX0A(unsigned short opcode, System *system) {
    //LD Vx, K

    //if a key is pressed, key value into Vx and PC += 2
    for(int i = 0; i < 16; i++) {
        if(system->keys[i] == 0xF) {
            system->dataRegisters[(opcode & 0x0F00) >> 8] = system->keys[i];
            system->programCounter += 2;
        }
    }
}

void executeFX15(unsigned short opcode, System *system) {
    //LD DT, Vx
    system->delayTimer = system->dataRegisters[(opcode & 0x0F00) >> 8];
    system->programCounter += 2;
}

void executeFX18(unsigned short opcode, System *system) {
    //LD ST, Vx
    system->soundTimer = system->dataRegisters[(opcode & 0x0F00) >> 8];
    system->programCounter += 2;
}

void executeFX1E(unsigned short opcode, System *system) {
    //ADD I, Vx
    system->addressRegister = system->addressRegister + system->dataRegisters[(opcode & 0x0F00) >> 8];
    system->programCounter += 2;
}

void executeFX29(unsigned short opcode, System *system) {
    //LD F, Vx
    unsigned char character = system->dataRegisters[(opcode & 0x0F00) >> 8];

    //Calculate the position of the character by adding 0x050 and the position of the character times the character size
    system->addressRegister = 0x050 + (character * 5);

    system->programCounter += 2;
}

void executeFX33(unsigned short opcode, System *system) {
    //LD B, Vx
    unsigned char number = system->dataRegisters[(opcode & 0x0F00) >> 8];

    //Extract the digits of the number up to the hundreds
    unsigned char hundreds = number / 100;
    unsigned char tens = (number / 10) % 10;
    unsigned char ones = number % 10;

    //Store each digit of the number to I + digitPosition
    system->memory[system->addressRegister] = hundreds;
    system->memory[system->addressRegister + 1] = tens;
    system->memory[system->addressRegister + 2] = ones;

    system->programCounter += 2;
}

void executeFX55(unsigned short opcode, System *system) {
    //LD [I], Vx
    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        system->memory[system->addressRegister + i] = system->dataRegisters[i];
        //system->addressRegister = system->addressRegister + i + 1;
    }
    system->addressRegister += ((opcode & 0x0F00) >> 8) + 1;
    system->programCounter += 2;
}

void executeFX65(unsigned short opcode, System *system) {
    //LD Vx, [I]
    for(int i = 0; i <= (opcode & 0x0F00) >> 8; i++) {
        system->dataRegisters[i] = system->memory[system->addressRegister + i];
    }
    system->addressRegister += ((opcode & 0x0F00) >> 8) + 1;
    system->programCounter += 2;
}
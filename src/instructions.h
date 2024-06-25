#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

void execute0NNN(unsigned short opcode, System *system);
void execute00E0(unsigned short opcode, System *system);
void execute00EE(unsigned short opcode, System *system);
void execute1NNN(unsigned short opcode, System *system);
void execute2NNN(unsigned short opcode, System *system);
void execute3XKK(unsigned short opcode, System *system);
void execute4XKK(unsigned short opcode, System *system);
void execute5XY0(unsigned short opcode, System *system);
void execute6XKK(unsigned short opcode, System *system);
void execute7XKK(unsigned short opcode, System *system);
void execute8XY0(unsigned short opcode, System *system);
void execute8XY1(unsigned short opcode, System *system);
void execute8XY2(unsigned short opcode, System *system);
void execute8XY3(unsigned short opcode, System *system);
void execute8XY4(unsigned short opcode, System *system);
void execute8XY5(unsigned short opcode, System *system);
void execute8XY6(unsigned short opcode, System *system);
void execute8XY7(unsigned short opcode, System *system);
void execute8XYE(unsigned short opcode, System *system);
void execute9XY0(unsigned short opcode, System *system);
void executeANNN(unsigned short opcode, System *system);
void executeBNNN(unsigned short opcode, System *system);
void executeCXKK(unsigned short opcode, System *system);
void executeDXYN(unsigned short opcode, System *system);
void executeEX9E(unsigned short opcode, System *system);
void executeEXA1(unsigned short opcode, System *system);
void executeFX07(unsigned short opcode, System *system);
void executeFX0A(unsigned short opcode, System *system);
void executeFX15(unsigned short opcode, System *system);
void executeFX18(unsigned short opcode, System *system);
void executeFX1E(unsigned short opcode, System *system);
void executeFX29(unsigned short opcode, System *system);
void executeFX33(unsigned short opcode, System *system);
void executeFX55(unsigned short opcode, System *system);
void executeFX65(unsigned short opcode, System *system);

#endif
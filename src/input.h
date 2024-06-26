#ifndef INPUT_H
#define INPUT_H

struct KeypadPair {
    SDL_Scancode scanCode;
    unsigned char key;
};

void handle_input(SDL_Event *event, System *system, struct KeypadPair keyTable[16]);

#endif
#include <SDL2/SDL.h>
#include "cpu.h"
#include "input.h"

void handle_input(SDL_Event *event, System *system, struct KeypadPair keyTable[16]) {
    if(event->type == SDL_KEYDOWN || event->type == SDL_KEYUP) {
        SDL_Scancode scanCode = event->key.keysym.scancode;
        int newState = (event->type == SDL_KEYDOWN) ? 0xF : 0x0; // 0xF if keydown, 0x0 if keyup

        for(int i = 0; i < 16; i++) {
            if(scanCode == keyTable[i].scanCode) {
                system->keys[keyTable[i].key] = newState;
                break; // Exit the loop once the matching key is found
            }
        }
    }
}
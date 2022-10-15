#include <stdio.h>
#include "lcd.h"


static SDL_Window *window;
static SDL_Surface *surface;


uint8_t lcd_load(void) {
    SDL_Init(SDL_INIT_EVERYTHING);

    window = SDL_CreateWindow(
        "yobemag GB Emulator",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 576,
        SDL_WINDOW_INPUT_FOCUS
    );

    surface = SDL_GetWindowSurface(window);

    return 0;
}

uint8_t lcd_step(void) {
    SDL_Event e;
    const uint8_t *key_states;

	key_states = SDL_GetKeyboardState(NULL);

    while(SDL_PollEvent(&e)) {
        if(e.type == SDL_QUIT) {
            return 0;
        }
    }

    if(key_states[SDL_SCANCODE_Q]) {
        return 1;
    }


    if(key_states[SDL_SCANCODE_A]) {
        LOG_DEBUG("a");
    }

    return 0;
}

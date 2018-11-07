#include "SDLEventsProcessor.h"
#include "Core.h"

#include "SDL2/SDL.h"

void SDLEventsProcessor::generateEvents(int32_t deltaTime)
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent)) {
        switch (sdlEvent.type) {
        case SDL_QUIT:
            Core::getCore()->requestExit();
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            SDL_Keycode keyCode = sdlEvent.key.keysym.sym;
            if (keyCode >= SDLK_a && keyCode <= SDLK_z) {
                KeyEvent event;
                event.key = static_cast<Key>(keyCode - SDLK_a);
                event.keyState = sdlEvent.type == SDL_KEYDOWN
                    ? KeyState::PRESSED
                    : KeyState::RELEASED;

                pushEvent(event);
            }
        } break;
        }
    }
}

#include "SDLWindow.h"
#include "Core.h"
#include "Exception.h"
#include "SDL2/SDL.h"
#include "Scene.h"

#include <chrono>
#include <iostream>

SDLWindow::SDLWindow(const unsigned width, const unsigned height)
    : width(width)
    , height(height)
{
}

void SDLWindow::onModuleCreated()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        throw Exception(
            std::string("Unable to init sdl. Error: ")
            + SDL_GetError());
    }

    sdlWindow = SDL_CreateWindow("Title", 0, 0, 640, 480, SDL_WINDOW_SHOWN);
    if (sdlWindow == nullptr) {
        throw Exception(
            std::string("Unable to create window. Error: ")
            + SDL_GetError());
    }

    sdlRenderer = SDL_CreateRenderer(
        sdlWindow,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (sdlRenderer == nullptr) {
        throw Exception(
            std::string("Unable to create renderer. Error: ")
            + SDL_GetError());
    }
}

void SDLWindow::processFrame(int32_t)
{
    processEvents();
}

SDLWindow::~SDLWindow()
{
    if (sdlWindow)
        SDL_DestroyWindow(sdlWindow);
    if (sdlRenderer)
        SDL_DestroyRenderer(sdlRenderer);
}

SDL_Renderer* SDLWindow::getSdlRenderer() const
{
    return sdlRenderer;
}

unsigned SDLWindow::getWidth() const
{
    return width;
}

unsigned SDLWindow::getHeight() const
{
    return height;
}

void SDLWindow::subscribeOnKeyPress(SDL_Keycode keyCode, const std::function<void()>& callback)
{
    keyPressSubscriptions[keyCode].push_back(callback);
}

void SDLWindow::subscribeOnKeyRelease(SDL_Keycode keyCode, const std::function<void()>& callback)
{
    keyReleaseSubscriptions[keyCode].push_back(callback);
}

void SDLWindow::processEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            Core::getCore()->requestExit();
            break;
        case SDL_KEYDOWN: {
            SDL_Keycode keyCode = event.key.keysym.sym;
            if (keyCode == SDLK_ESCAPE) {
                Core::getCore()->requestExit();
            }

            auto it = keyPressSubscriptions.find(keyCode);
            if (it != keyPressSubscriptions.end() && !keyStates[keyCode]) {
                for (const auto& function : it->second) {
                    function();
                }
            }
            keyStates[keyCode] = true;
        } break;
        case SDL_KEYUP: {
            SDL_Keycode keyCode = event.key.keysym.sym;
            auto it = keyReleaseSubscriptions.find(keyCode);
            if (it != keyReleaseSubscriptions.end()) {
                for (const auto& function : it->second) {
                    function();
                }
            }
            keyStates[keyCode] = false;
        } break;
        }
    }
}

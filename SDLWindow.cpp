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

void SDLWindow::onModuleCreation()
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
    // processEvents();
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

#pragma once

#include "Module.h"
#include "SDL_keycode.h"

#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

struct SDL_Window;
struct SDL_Renderer;

class SDLWindow : public Module {
public:
    SDLWindow(const unsigned width, const unsigned height);
    virtual ~SDLWindow() override;
    virtual void onModuleCreation() override;
    virtual void processFrame(int32_t deltaTime) override;

    SDL_Renderer* getSdlRenderer() const;

    unsigned getWidth() const;

    unsigned getHeight() const;

private:
    // void processEvents();

    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    const unsigned width, height;
};

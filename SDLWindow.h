#pragma once

#include "Module.h"
#include "SDL_keycode.h"
#include "SceneRenderer.h"

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
    virtual void onModuleCreated() override;
    virtual void processFrame(int32_t deltaTime) override;

    std::shared_ptr<Scene> getScene() const;

    SDL_Renderer* getSdlRenderer() const;

    unsigned getWidth() const;

    unsigned getHeight() const;

    void subscribeOnKeyPress(SDL_Keycode keyCode, const std::function<void()>& callback);

    void subscribeOnKeyRelease(SDL_Keycode keyCode, const std::function<void()>& callback);

private:
    void processEvents();

    SDL_Window* sdlWindow;
    SDL_Renderer* sdlRenderer;
    const unsigned width, height;
    std::unordered_map<SDL_Keycode, std::vector<std::function<void()>>> keyPressSubscriptions;
    std::unordered_map<SDL_Keycode, std::vector<std::function<void()>>> keyReleaseSubscriptions;
    std::unordered_map<SDL_Keycode, bool> keyStates;
};

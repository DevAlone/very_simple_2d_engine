#pragma once

#include "Vector.h"

#include <cstddef>
#include <memory>
#include <unordered_set>

class SDL_Texture;
class SDL_Renderer;
class SDLWindow;

class GameObject {
public:
    GameObject(
        const Vector2F& position,
        const Vector2F& size);

    virtual ~GameObject();

    void addChild(const std::shared_ptr<GameObject>& child);

    const std::unordered_set<std::shared_ptr<GameObject>>& getChildren() const;

    const Vector2F& getPosition() const;

    Vector2F& getPositionRef();

    void setPosition(const Vector2F& value);

    void setTextureFromFile(const std::string& filename, SDL_Renderer* renderer);

    SDL_Texture* getTexture() const;

    const Vector2F& getSize() const;

    void setSize(const Vector2F& value);

    Vector2F getSpeed() const;
    Vector2F& getSpeedRef();

    void setSpeed(const Vector2F& value);

    void addSpeed(const Vector2F& value);

private:
    std::unordered_set<std::shared_ptr<GameObject>> children;
    Vector2F position;
    Vector2F speed;
    Vector2F size;
    // Vector<PositionType, Dimensions> speed;
    SDL_Texture* texture = nullptr;
};

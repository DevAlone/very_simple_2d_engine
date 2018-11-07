#include "GameObject.h"
#include "Exception.h"
#include "SDLWindow.h"

#include "SDL2/SDL.h"
#include "SDL_image.h"

using namespace game_math;

GameObject::GameObject(const Vector2F& position, const Vector2F& size)
    : position(position)
    , size(size)
{
}

GameObject::~GameObject()
{
    if (texture)
        SDL_DestroyTexture(texture);
}

void GameObject::addChild(const std::shared_ptr<GameObject>& child)
{
    children.insert(child);

    if (auto p = child->getParent()) {
        p->removeChild(child);
    }

    child->parent = this;
}

void GameObject::removeChild(const std::shared_ptr<GameObject>& child)
{
    if (auto p = child->getParent()) {
        p->parent = nullptr;
    }
    children.erase(child);
}

const std::unordered_set<std::shared_ptr<GameObject>>& GameObject::getChildren() const
{
    return children;
}

const Vector2F& GameObject::getPosition() const
{
    return position;
}

Vector2F& GameObject::getPositionRef()
{
    return position;
}

void GameObject::setPosition(const Vector2F& value)
{
    position = value;
}

void GameObject::setTextureFromFile(const std::string& filename, SDL_Renderer* renderer)
{
    if (texture)
        SDL_DestroyTexture(texture);

    texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture == nullptr) {
        throw Exception(
            std::string("Unable to create texture. Error: ")
            + SDL_GetError());
    }
}

SDL_Texture* GameObject::getTexture() const
{
    return texture;
}

const Vector2F& GameObject::getSize() const
{
    return size;
}

void GameObject::setSize(const Vector2F& value)
{
    size = value;
}

Vector2F GameObject::getSpeed() const
{
    return speed;
}

Vector2F& GameObject::getSpeedRef()
{
    return speed;
}

void GameObject::setSpeed(const Vector2F& value)
{
    speed = value;
}

void GameObject::addSpeed(const Vector2F& value)
{
    speed += value;
}

GameObject* GameObject::getParent() const
{
    return parent;
}

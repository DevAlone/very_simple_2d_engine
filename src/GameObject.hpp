#pragma once

class SDL_Texture;
class SDL_Renderer;
class SDLWindow;

#include "Exception.h"
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "game_math/Vector.hpp"

#include <cstddef>
#include <memory>
#include <unordered_set>
#include <vector>

/**
 * @brief GameObject reperesents any object inside the scene, including UI elements
 * can contain children
 */
template <std::size_t Size, typename Type>
class GameObject {

public:
    virtual ~GameObject();

    auto addChild(const std::shared_ptr<GameObject>& child) -> void;
    auto removeChild(const std::shared_ptr<GameObject>& child) -> void;
    auto getChildren() const -> const std::unordered_set<std::shared_ptr<GameObject>>&;
    auto setPosition(const game_math::Vector2F& value) -> void;
    auto setTextureFromFile(const std::string& filename, SDL_Renderer* renderer) -> void;
    auto getTexture() const -> SDL_Texture*;
    auto getParent() const -> GameObject*;

private:
    /**
     * @brief objectsTreeContainItem - looks for object in tree
     * @param root - tree root
     * @param object - object to find
     * @return true if object found false if not
     */
    static auto objectsTreeContainsItem(
        const std::shared_ptr<GameObject>& root,
        const std::shared_ptr<GameObject>& object) -> bool;

    GameObject* parent = nullptr;
    std::unordered_set<std::shared_ptr<GameObject>> children;
    SDL_Texture* texture = nullptr;
};

using GameObject2F = GameObject<2, float>;

template <std::size_t Size, typename Type>
GameObject<Size, Type>::~GameObject()
{
    if (texture)
        SDL_DestroyTexture(texture);

    for (const auto& child : children) {
        child->parent = nullptr;
    }
}

template <std::size_t Size, typename Type>
void GameObject<Size, Type>::addChild(const std::shared_ptr<GameObject>& child)
{
    children.insert(child);

    if (auto p = child->getParent()) {
        p->removeChild(child);
    }

    child->parent = this;

    std::vector<std::shared_ptr<GameObject<Size, Type>>> grandchildrenToRemove;
    for (const auto& grandchild : child->getChildren()) {
        if (objectsTreeContainsItem(grandchild, child)) {
            grandchildrenToRemove.push_back(grandchild);
        }
    }
    for (const auto& grandchild : grandchildrenToRemove) {
        child->removeChild(grandchild);
    }
}

template <std::size_t Size, typename Type>
void GameObject<Size, Type>::removeChild(const std::shared_ptr<GameObject>& child)
{
    child->parent = nullptr;
    children.erase(child);
}

template <std::size_t Size, typename Type>
const std::unordered_set<std::shared_ptr<GameObject<Size, Type>>>& GameObject<Size, Type>::getChildren() const
{
    return children;
}

template <std::size_t Size, typename Type>
void GameObject<Size, Type>::setTextureFromFile(const std::string& filename, SDL_Renderer* renderer)
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

template <std::size_t Size, typename Type>
SDL_Texture* GameObject<Size, Type>::getTexture() const
{
    return texture;
}

template <std::size_t Size, typename Type>
GameObject<Size, Type>* GameObject<Size, Type>::getParent() const
{
    return parent;
}

template <std::size_t Size, typename Type>
bool GameObject<Size, Type>::objectsTreeContainsItem(
    const std::shared_ptr<GameObject>& root,
    const std::shared_ptr<GameObject>& object)
{
    if (!root || !object)
        return false;

    if (root == object)
        return true;

    for (const auto& child : root->getChildren()) {
        if (GameObject<Size, Type>::objectsTreeContainsItem(child, object)) {
            return true;
        }
    }

    return false;
}

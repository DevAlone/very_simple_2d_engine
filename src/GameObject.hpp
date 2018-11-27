#pragma once

class SDL_Texture;
class SDL_Renderer;
class SDLWindow;

#include <cstddef>

template <std::size_t Size, typename Type>
class Scene;

#include "Exception.h"
#include "SDL2/SDL.h"
#include "SDL_image.h"
#include "game_math/Vector.hpp"

#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

/**
 * @brief GameObject reperesents any object inside the scene, including UI elements
 * can contain children
 */
template <std::size_t Size, typename Type>
class GameObject {
    template <std::size_t S, typename T>
    friend class Scene;

    using String = std::string;

public:
    virtual ~GameObject();

    auto addChild(const std::shared_ptr<GameObject>& child) -> void;
    auto removeChild(const std::shared_ptr<GameObject>& child) -> void;
    auto getChildren() -> std::unordered_set<std::shared_ptr<GameObject>>&;
    auto setTextureFromFile(const std::string& filename, SDL_Renderer* renderer) -> void;
    auto getTexture() const -> SDL_Texture*;
    auto getParent() const -> GameObject*;
    auto getColor() const -> game_math::Vector<4, unsigned char>;

    void setColor(
        unsigned char red,
        unsigned char green,
        unsigned char blue,
        unsigned char alpha = 255);

    auto getName() const -> String
    {
        return name;
    }
    void setName(const String& value)
    {
        name = value;
    }

protected:
    std::shared_ptr<Scene<Size, Type>> scene;

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

    void setSceneRecursively(const std::shared_ptr<Scene<Size, Type>>& value);

    String name;
    GameObject* parent = nullptr;
    std::unordered_set<std::shared_ptr<GameObject>> children;
    SDL_Texture* texture = nullptr;
    game_math::Vector<4, unsigned char> color
        = game_math::Vector<4, unsigned char> { 0, 0, 0, 0 };
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
    child->setSceneRecursively(scene);

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
auto GameObject<Size, Type>::getChildren()
    -> std::unordered_set<std::shared_ptr<GameObject<Size, Type>>>&
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

template <std::size_t Size, typename Type>
void GameObject<Size, Type>::setSceneRecursively(const std::shared_ptr<Scene<Size, Type>>& value)
{
    scene = value;
    for (const auto& child : children) {
        child->setSceneRecursively(scene);
    }
}

template <std::size_t Size, typename Type>
game_math::Vector<4, unsigned char> GameObject<Size, Type>::getColor() const
{
    return color;
}

template <std::size_t Size, typename Type>
void GameObject<Size, Type>::setColor(
    unsigned char red,
    unsigned char green,
    unsigned char blue,
    unsigned char alpha)
{
    color = game_math::Vector<4, unsigned char> { red, green, blue, alpha };
}

template <std::size_t nDimensions, typename BaseType>
std::ostream& operator<<(
    std::ostream& stream,
    const std::shared_ptr<GameObject<nDimensions, BaseType>>& obj)
{
    return stream
        << "GameObjectPtr { name: \""
        << obj->getName()
        << "\", address: \""
        << obj.get()
        << "\" }";
}

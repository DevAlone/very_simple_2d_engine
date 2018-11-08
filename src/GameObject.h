#pragma once

#include "game_math/Vector.hpp"

#include <cstddef>
#include <memory>
#include <unordered_set>

class SDL_Texture;
class SDL_Renderer;
class SDLWindow;

/**
 * @brief GameObject reperesents any object inside the scene, including UI elements
 * can contain children which will have coordinates from this object
 */
class GameObject {

public:
    GameObject(
        const game_math::Vector2F& position,
        const game_math::Vector2F& size);

    virtual ~GameObject();

    void addChild(const std::shared_ptr<GameObject>& child);

    void removeChild(const std::shared_ptr<GameObject>& child);

    const std::unordered_set<std::shared_ptr<GameObject>>& getChildren() const;

    const game_math::Vector2F& getPosition() const;

    game_math::Vector2F& getPositionRef();

    void setPosition(const game_math::Vector2F& value);

    void setTextureFromFile(const std::string& filename, SDL_Renderer* renderer);

    SDL_Texture* getTexture() const;

    const game_math::Vector2F& getSize() const;

    void setSize(const game_math::Vector2F& value);

    game_math::Vector2F getSpeed() const;
    game_math::Vector2F& getSpeedRef();

    void setSpeed(const game_math::Vector2F& value);

    void addSpeed(const game_math::Vector2F& value);

    GameObject* getParent() const;

private:
    /**
     * @brief objectsTreeContainItem - looks for object in tree
     * @param root - tree root
     * @param object - object to find
     * @return true if object found false if not
     */
    static bool objectsTreeContainsItem(
        const std::shared_ptr<GameObject>& root,
        const std::shared_ptr<GameObject>& object);

    GameObject* parent = nullptr;
    std::unordered_set<std::shared_ptr<GameObject>> children;
    game_math::Vector2F position;
    game_math::Vector2F speed;
    game_math::Vector2F size;
    // Vector<PositionType, Dimensions> speed;
    SDL_Texture* texture = nullptr;
};

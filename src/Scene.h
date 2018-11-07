#ifndef SCENE_H
#define SCENE_H

class GameObject;

#include "Module.h"

#include <cstdint>
#include <memory>

class Scene : public Module {
public:
    Scene();

    std::shared_ptr<GameObject> getRootObject() const;
    void setRootObject(const std::shared_ptr<GameObject>& value);

private:
    std::shared_ptr<GameObject> rootObject;
};

#endif // SCENE_H

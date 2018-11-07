#include "Scene.h"

#include <iostream>

Scene::Scene()
{
}

std::shared_ptr<GameObject> Scene::getRootObject() const
{
    return rootObject;
}

void Scene::setRootObject(const std::shared_ptr<GameObject>& value)
{
    rootObject = value;
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/GameObject.h"
#include "../src/game_math/game_math.h"

#include <memory>

using namespace game_math;

TEST_CASE("Vectors operations", "[Vector]")
{
    CHECK(nearlyEqual(
        Vector2F { 1.5, 2.5 } * 2,
        Vector2F { 3, 5 }));

    CHECK(nearlyEqual(
        Vector2F { 1.5, 2.5 } * 2,
        Vector2F { 3, 5 }));

    CHECK(nearlyEqual(
        Vector2F { 3, 4 } / 2,
        Vector2F { 1.5, 2 }));

    CHECK(nearlyEqual(
        Vector2F { 1.5, 2.5 } + Vector2F(-1.5, -10),
        Vector2F { 0, -7.5 }));

    CHECK(nearlyEqual(
        Vector2F { -10, 0 } - Vector2F(-1.5, -10),
        Vector2F { -8.5, 10 }));
}

TEST_CASE("GameObjects reparenting", "[GameObject]")
{
    auto obj1 = std::make_shared<GameObject>(
        Vector2F(0, 0), Vector2F(0, 0));
    auto obj2 = std::make_shared<GameObject>(
        Vector2F(0, 0), Vector2F(0, 0));
    auto obj3 = std::make_shared<GameObject>(
        Vector2F(0, 0), Vector2F(0, 0));

    CHECK(obj1->getParent() == nullptr);
    CHECK(obj2->getParent() == nullptr);
    CHECK(obj3->getParent() == nullptr);

    CHECK(obj1->getChildren().size() == 0);
    CHECK(obj2->getChildren().size() == 0);
    CHECK(obj3->getChildren().size() == 0);

    obj1->addChild(obj2);
    obj2->addChild(obj3);

    CHECK(obj1->getParent() == nullptr);
    CHECK(obj2->getParent() == obj1.get());
    CHECK(obj3->getParent() == obj2.get());

    CHECK(obj1->getChildren().size() == 1);
    CHECK(obj2->getChildren().size() == 1);
    CHECK(obj3->getChildren().size() == 0);

    obj3->addChild(obj1);

    CHECK(obj1->getParent() == obj3.get());
    CHECK(obj2->getParent() == nullptr);
    CHECK(obj3->getParent() == obj2.get());

    CHECK(obj1->getChildren().size() == 1);
    CHECK(obj2->getChildren().size() == 0);
    CHECK(obj3->getChildren().size() == 1);
}

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../src/GameObject.hpp"
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
        Vector2F { 1.5, 2.5 } + Vector2F { -1.5, -10.0 },
        Vector2F { 0, -7.5 }));

    CHECK(nearlyEqual(
        Vector2F { -10, 0 } - Vector2F { -1.5, -10 },
        Vector2F { -8.5, 10 }));
}

TEST_CASE("GameObjects reparenting", "[GameObject]")
{
    auto obj1 = std::make_shared<GameObject<2, float>>();
    auto obj2 = std::make_shared<GameObject<2, float>>();
    auto obj3 = std::make_shared<GameObject<2, float>>();

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

    CHECK(obj1->getChildren().size() == 0);
    CHECK(obj2->getChildren().size() == 1);
    CHECK(obj3->getChildren().size() == 1);

    obj1 = std::make_shared<GameObject<2, float>>();
    obj2 = std::make_shared<GameObject<2, float>>();

    obj1->addChild(obj2);

    CHECK(obj1->getParent() == nullptr);
    CHECK(obj2->getParent() == obj1.get());

    obj2->addChild(obj1);

    CHECK(obj1->getParent() == obj2.get());
    CHECK(obj2->getParent() == nullptr);
}

TEST_CASE("Matrices operations", "[Matrix]")
{
    game_math::Matrix<3, 3, int> m0({
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    });

    m0[0][0] = 99;
    m0[1][1] = -5;
    m0[1][2] = -4;

    CHECK(m0 == decltype(m0)({
                    { 99, 2, 3 },
                    { 4, -5, -4 },
                    { 7, 8, 9 },
                }));

    Matrix<2, 2, int> m1({
        { 1, 2 },
        { 3, 4 },
    });

    CHECK(m1[0][0] == 1);
    CHECK(m1[0][1] == 2);
    CHECK(m1[1][0] == 3);
    CHECK(m1[1][1] == 4);

    m1.transpose();

    CHECK(m1[0][0] == 1);
    CHECK(m1[0][1] == 3);
    CHECK(m1[1][0] == 2);
    CHECK(m1[1][1] == 4);

    CHECK(m1 == Matrix<2, 2, int>({
                    { 1, 3 },
                    { 2, 4 },
                }));

    m1.rotateClockwise();

    CHECK(m1 == decltype(m1)({
                    { 2, 1 },
                    { 4, 3 },
                }));

    auto m2 = Matrix<4, 4, int>({
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 },
        { 13, 14, 15, 16 },
    });

    m2.rotateClockwise();

    CHECK(m2 == decltype(m2)({
                    { 13, 9, 5, 1 },
                    { 14, 10, 6, 2 },
                    { 15, 11, 7, 3 },
                    { 16, 12, 8, 4 },
                }));

    m2.rotateClockwise();
    m2.rotateClockwise();
    m2.rotateClockwise();

    CHECK(m2 == decltype(m2)({
                    { 1, 2, 3, 4 },
                    { 5, 6, 7, 8 },
                    { 9, 10, 11, 12 },
                    { 13, 14, 15, 16 },
                }));

    auto m3 = Matrix<3, 3, int>({
        { 1, 2, 3 },
        { 4, 5, 6 },
        { 7, 8, 9 },
    });
    m3.rotateCounterclockwise();
    m3.rotateCounterclockwise();

    CHECK(m3 == decltype(m3)({
                    { 9, 8, 7 },
                    { 6, 5, 4 },
                    { 3, 2, 1 },
                }));
    m3.rotateCounterclockwise();
    m3.rotateCounterclockwise();

    CHECK(m3 == decltype(m3)({
                    { 1, 2, 3 },
                    { 4, 5, 6 },
                    { 7, 8, 9 },
                }));
}

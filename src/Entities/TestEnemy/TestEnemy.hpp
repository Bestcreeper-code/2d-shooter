#pragma once

#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"
#include "Sprites/AnimatedSprite.hpp"


class TestEnemy : public PhysicsObject {
public:
    const ObjectType getType() override {
        return ObjectType::OBJ_TYPE_ENEMY;
    }



    float speed;
    float shoot_cooldown = 0.5f;
    float time_until_can_shoot = 0.0f;
    
    float health = 100.0f;

    Sprite sprite;
    TestEnemy();

    
    void Draw() override;
    void Update() override;
    void onCollision(PhysicsObject *other) override;
};
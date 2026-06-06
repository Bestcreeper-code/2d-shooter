#pragma once

#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"
#include "Sprites/AnimatedSprite.hpp"


class TestEnemy : public PhysicsObject {
    HealthBar healthBar;
public:
    const ObjectType getType() override {
        return ObjectType::OBJ_TYPE_ENEMY;
    }



    float speed;
    float shoot_cooldown = 0.5f;
    float time_until_can_shoot = 0.0f;
    
    float max_health = 100.0f;
    float health;

    Sprite sprite;
    TestEnemy(Vector2 pos);
    ~TestEnemy();

    
    void Draw() override;
    void Update(float deltaTime) override;
    void onCollision(PhysicsObject *other) override;
};
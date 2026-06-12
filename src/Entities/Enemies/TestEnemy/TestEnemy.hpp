#pragma once

#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"
#include "Sprites/AnimatedSprite.hpp"
#include <cstdint>


class TestEnemy : public PhysicsObject {
private:
    HealthBar healthBar;
    bool give_point;
    void Die(uint8_t points);
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

    
    void Init() override;
    void Draw() override;
    void Update(float deltaTime) override;
    void onCollision(PhysicsObject *other) override;
};
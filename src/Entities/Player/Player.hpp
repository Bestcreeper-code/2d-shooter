#pragma once

#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "Sprites/AnimatedSprite.hpp"


class Player : public PhysicsObject {
private:
    HealthBar healthBar;
    bool was_hit_this_frame = false;
public:
    const ObjectType getType() override {
        return ObjectType::OBJ_TYPE_PLAYER;
    }

    float speed;
    float shoot_cooldown = 0.2f;

    float max_health;
    float health;

    AnimatedSprite sprite;
    Player();
    ~Player();

    void Init() override;
    void Draw() override;
    void Update(float deltaTime) override;
    void onCollision(PhysicsObject *other) override;
    
    void Die();
};
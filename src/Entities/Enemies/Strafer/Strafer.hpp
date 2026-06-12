#pragma once
#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"

class StraferEnemy : public PhysicsObject {
private:
    enum class Phase { Drop, Strafe };
    Phase phase = Phase::Drop;

    int dir = 1;
    float areaLeft = 10.f;
    float areaRight = 0.f;
    bool hasEntered = false;

    HealthBar healthBar;
    bool give_point = true;

    void Die(uint8_t points);
public:
    const ObjectType getType() override { return ObjectType::OBJ_TYPE_ENEMY; }

    float max_health = 80.f;
    float health;
    float speed = 120.f;
    float dropSpeed = 200.f;
    float strafe_y;          
    float shoot_cooldown = 0.8f;
    float time_until_can_shoot = 0.f;

    Sprite sprite;
    StraferEnemy(Vector2 startPos, float strafe_y);
    ~StraferEnemy();

    void Init() override;
    void Draw() override;
    void Update(float dt) override;
    void onCollision(PhysicsObject* other) override;

};
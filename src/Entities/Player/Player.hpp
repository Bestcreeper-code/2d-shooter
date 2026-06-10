// Player.hpp
#pragma once

#include "Bullets/Bullet.hpp"
#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "Sprites/AnimatedSprite.hpp"
#include "raylib.h"
#include <map>
#include <unordered_map>
#include <vector>

struct BulletSpawnerData {
    BulletType type     = BULLET_TYPE_STANDARD;
    float      cooldown = 0.2f;
    Vector2    offset   = {0.0f, -10.0f};

    float _timer = 0.0f;
};


class Player : public PhysicsObject {
private:
    HealthBar healthBar;
    bool was_hit_this_frame = false;
    std::map<int, BulletSpawnerData> weapons;
    int _nextSpawnerId = 0;


public:
    const ObjectType getType() override {
        return ObjectType::OBJ_TYPE_PLAYER;
    }

    float speed;
    float max_health;
    float health;

    AnimatedSprite sprite;
    Player();
    ~Player();

    int  AddWeapon(BulletType type,
        float cooldown,
        Vector2 offset = {0.0f, -10.0f});

    BulletSpawnerData* GetWeapon(int id);
    void           RemoveWeapon(int id);

    void Init() override;
    void Draw() override;
    void Update(float deltaTime) override;
    void onCollision(PhysicsObject* other) override;

    void Die();
};
#pragma once


#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"
#include "raylib.h"


class Bullet : public PhysicsObject 
{
    private:
        bool isPlayerBullet;
        
    public:

        const ObjectType getType() override {
            return isPlayerBullet ? ObjectType::OBJ_TYPE_PLAYER_BULLET : ObjectType::OBJ_TYPE_ENEMY_BULLET;
        }
    
        float speed;
        Sprite sprite;

        float damage = 10.0f;
        
        Bullet(bool isPlayerBullet, Vector2 position, float dmg);
        ~Bullet();

        void Init() override{}
        void Update(float deltaTime) override;
        void Draw() override;
        void onCollision(PhysicsObject* other) override;
};
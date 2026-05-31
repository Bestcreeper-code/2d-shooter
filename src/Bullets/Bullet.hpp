#pragma once


#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"


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
        
        Bullet(bool isPlayerBullet, px_Vec2 position, float dmg);
        ~Bullet();

        void Draw() override;
        void Update(float deltaTime) override;
        void onCollision(PhysicsObject* other) override;
};
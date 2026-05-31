#pragma once

#include "Collision/Collisions.hpp"
#include "raylib.h"
#include <cstdint>

enum ObjectType : uint8_t {
    OBJ_TYPE_PLAYER,
    OBJ_TYPE_PLAYER_BULLET,

    OBJ_TYPE_ENEMY,
    OBJ_TYPE_ENEMY_BULLET,

    OBJ_TYPE_PLAYER_BONUS,

    OBJ_TYPE_GROUND,
};

class Actor {
    public:
        bool pendingDelete = false;

        virtual void Update(float deltaTime) = 0;
        virtual void Draw() {}
        virtual ~Actor() = default;

        
};

class PhysicsObject : public Actor {
    public:

        virtual const ObjectType getType() = 0;

        BodyResult body;
    
        virtual ~PhysicsObject() = default;
    
        



        virtual void onCollision(PhysicsObject* other) {}
};

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


class PhysicsObject {
    public:
        bool pendingDelete = false;

        virtual const ObjectType getType() = 0;

        BodyResult body;
    
        virtual ~PhysicsObject() = default;
    
        

        virtual void Draw() = 0;

        virtual void Update() =0;

        virtual void onCollision(PhysicsObject* other) {}
};

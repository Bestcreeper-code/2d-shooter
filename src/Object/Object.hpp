#pragma once

#include "box2d/box2d.h"
class Actor;

#include "Collision/Collisions.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
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
        ActorId actor_id;


        bool pendingDelete = false;
        bool pausable = true;
        uint8_t drawing_layer = 0;

        virtual void Init() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Draw() {}
        virtual ~Actor() = default;

        
};

class PhysicsObject : public Actor {
    public:

        virtual const ObjectType getType() = 0;

        BodyResult body;
    
        virtual ~PhysicsObject() = default;
    
        
        void Init() override {


    
            b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
        }


        virtual void onCollision(PhysicsObject* other) {}
};

#pragma once

#include "box2d/box2d.h"
class Actor;

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
    OBJ_TYPE_INVALID,
};
#include "ObjectMgr/ObjectMgr.hpp"



class Actor {
    public:
        ActorId actor_id;


        bool pendingDelete = false;
        bool pausable = true;
        uint8_t drawing_layer = 0;

        virtual void PreInit() {}
        virtual void Init() {}
        virtual void Update(float deltaTime) {}
        virtual void Draw() {}
        virtual ~Actor() = default;
};

class PhysicsObject : public Actor {
    public:

        virtual const ObjectType getType() {return ObjectType::OBJ_TYPE_INVALID;};

        BodyResult body;
    
        virtual ~PhysicsObject() = default;
    
        
        void PreInit() override {  
            b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
        }


        virtual void onCollision(PhysicsObject* other) {}
};

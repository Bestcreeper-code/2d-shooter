#pragma once

#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "box2d/math_functions.h"
#include "config.h"
#include "config.hpp"


class Wall : public PhysicsObject {
    private:
        float width,height;

    public:
        const ObjectType getType() override {
            return ObjectType::OBJ_TYPE_GROUND;
        }

        Wall(b2Vec2 position, float width, float height);

        
    
        void Draw() override;

        void Update() override {}
};
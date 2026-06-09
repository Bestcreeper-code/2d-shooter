#include "Platforms/Wall/Wall.hpp"
#include "Collision/Collisions.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "config.hpp"
#include <cstdio>




Wall::Wall(b2Vec2 position, float width, float height) {
    
    b2Filter filter;
    filter.categoryBits = COLLISION_LAYER_GROUND;
    filter.maskBits = COLLISION_LAYER_PLAYER 
                                    | COLLISION_LAYER_PLAYER_BULLET 
                                    | COLLISION_LAYER_ENEMY_BULLET
                                    | COLLISION_LAYER_ENEMY;
    
    this->width = width;
    this->height = height;

    body = CreateBoxBody(
        gWorld,
        {PX_2_M(position.x),PX_2_M(position.y)},
        PX_2_M(width),
        PX_2_M(height),
        0.1f,
        1.0f,
        0.0f,
        false,
        filter
    );
}

void Wall::Draw() 
{
    b2Transform t = b2Body_GetTransform(body.bodyId);
    
    DrawRectangle(
        M_2_PX(t.p.x)-width,
        M_2_PX(t.p.y)-height,
        width*2,
        height*2,
        DARKGRAY
    );
}

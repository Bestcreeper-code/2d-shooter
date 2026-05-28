#pragma once 

#include "box2d/box2d.h"
#include "box2d/collision.h"

#define PPM 100.0f //1cm per px

inline float PX_2_M(float px) { return px / PPM; }
inline float M_2_PX(float m)  { return m * PPM; }



enum CollisionLayer {
    COLLISION_LAYER_PLAYER = 0x0001,
    COLLISION_LAYER_ENEMY  = 0x0002,
    
    COLLISION_LAYER_PLAYER_BULLET = 0x0004,
    COLLISION_LAYER_ENEMY_BULLET = 0x0008,

    COLLISION_LAYER_PLAYER_BONUS = 0x0010,

    COLLISION_LAYER_GROUND = 0x0020
};

extern b2WorldDef worldDef;
extern b2WorldId gWorld;

typedef b2Vec2 px_Vec2;
typedef b2Vec2 m_Vec2;

typedef struct BodyResult
{
    b2BodyId bodyId;
    b2ShapeId shapeId;
} BodyResult;

BodyResult CreateBoxBody(void* ownerptr, b2WorldId worldId, b2Vec2 position, 
    float hx, float hy, float density, float friction, float restitution, bool isDynamic, 
    b2Filter collFilter);

void ProcessCollisions();
#include "Collision/Collisions.hpp"
#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "box2d/box2d.h"
#include "box2d/types.h"
#include "config.hpp"
#include "macros.hpp"

#include <cstdint>
#include <cstdio>
#include <unordered_set>





b2WorldId gWorld;



BodyResult CreateBoxBody(
    b2WorldId worldId,
    b2Vec2 position,
    float hx,
    float hy,
    float density,
    float friction,
    float restitution,
    bool isDynamic,
    b2Filter collFilter
)
{

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = isDynamic ? b2_dynamicBody : b2_staticBody;
    bodyDef.position = position;
    
    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);

    // b2Body_SetUserData(bodyId, ownerptr);

    b2Polygon box = b2MakeBox(hx, hy);

    b2SurfaceMaterial material = b2DefaultSurfaceMaterial();
    material.friction = friction;

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    shapeDef.density = density;
    shapeDef.material = material;
    shapeDef.material.restitution = restitution;
    shapeDef.enableContactEvents = true;

    shapeDef.isSensor = false;
    shapeDef.enableSensorEvents = false;

    shapeDef.filter = collFilter;

    b2ShapeId shapeId = b2CreatePolygonShape(bodyId, &shapeDef, &box);

    if (density > 0.0f)
    {
        b2Body_ApplyMassFromShapes(bodyId);
    }

    BodyResult result = { bodyId, shapeId };
    return result;
}


void ProcessCollisions() {
    b2ContactEvents events = b2World_GetContactEvents(gWorld);
    for (int i = 0; i < events.beginCount; i++) {
        b2ContactBeginTouchEvent e = events.beginEvents[i];

        if (!b2Shape_IsValid(e.shapeIdA) || !b2Shape_IsValid(e.shapeIdB)) {
            errorf("Invalid shape in collision event");
            continue;
        } 

        b2BodyId bodyA = b2Shape_GetBody(e.shapeIdA);
        b2BodyId bodyB = b2Shape_GetBody(e.shapeIdB);

        if (!b2Body_IsValid(bodyA) || !b2Body_IsValid(bodyB))
        {
            errorf("Invalid body in collision event");
            continue;
        } 

        ActorId actorIdA = Uint64ToActorId((uint64_t)b2Body_GetUserData(bodyA));
        ActorId actorIdB = Uint64ToActorId((uint64_t)b2Body_GetUserData(bodyB));

        if (actorIdA.index == UINT32_MAX || actorIdA.generation == UINT32_MAX  ||
            actorIdB.index == UINT32_MAX || actorIdB.generation == UINT32_MAX) {

            errorf("Collision with body with non init ActorId");
            continue;
        } 

        Actor* actorA = GetActor(actorIdA);
        Actor* actorB = GetActor(actorIdB);

        PhysicsObject* a = dynamic_cast<PhysicsObject*>(actorA);
        PhysicsObject* b = dynamic_cast<PhysicsObject*>(actorB);

        if (!a || !b) {
            errorf("Collision with stray body a: %p, b: %p",a,b);
            continue;
        }
        if (a->pendingDelete || b->pendingDelete) continue;


#ifdef DEBUG_LOGS
        printf("Collision detected between actor %p and actor %p\n", (void*)a, (void*)b);
#endif
        a->onCollision(b);
        b->onCollision(a);
    }
}


void DestroyBody(BodyResult body) {
    if (!b2Body_IsValid(body.bodyId)) return;

    if (!b2Body_IsEnabled(body.bodyId)) {
        b2Body_Enable(body.bodyId);
    }

    b2DestroyBody(body.bodyId);
}
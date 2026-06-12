#include "Bullets/Bullet.hpp"
#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Textures/TextureCache.hpp"
#include "box2d/box2d.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "config.hpp"
#include "helpers.hpp"
#include "main.hpp"
#include "raymath.h"
#include <chrono>
#include <cstdio>
#include <thread>






// #error "fix the segfault"
Bullet::Bullet(bool isPlayerBullet, Vector2 position, float dmg) : isPlayerBullet(isPlayerBullet), damage(dmg) {
    
    
    speed = 8.0f;
    sprite.SetTexture(TextureCache::GetTexture(IMG_DIR"Bullet.png"));
    
    sprite.scale = 1.0f;
    
    
    body.collision_filter.categoryBits = isPlayerBullet ? COLLISION_LAYER_PLAYER_BULLET : COLLISION_LAYER_ENEMY_BULLET;
    body.collision_filter.maskBits = (isPlayerBullet? COLLISION_LAYER_ENEMY : COLLISION_LAYER_PLAYER) | COLLISION_LAYER_GROUND;
    body.collision_filter.groupIndex =-1;

    
    float halfW = std::max(PX_2_M(sprite.texture.width  * sprite.scale) / 2.0f, 0.01f);
    float halfH = std::max(PX_2_M(sprite.texture.height * sprite.scale) / 2.0f, 0.01f);

    body = CreateBoxBody(
        gWorld,
        {PX_2_M(position.x), PX_2_M(position.y)},
        halfW, halfH,
        1.0f, 0.3f, 0.8f,
        true, body.collision_filter
    );

    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 
    b2Body_SetBullet(body.bodyId, true);
}

Bullet::~Bullet() {
    DestroyBody(body);
}

void Bullet::Update(float deltaTime) {
    float curr_speed = (speed * deltaTime)*25;
    b2Vec2 velocity = {0.0f, isPlayerBullet ? -curr_speed : curr_speed};
    b2Body_SetLinearVelocity(body.bodyId, velocity);
}

void Bullet::Draw() {
    if (!b2Body_IsValid(body.bodyId)) return;
    b2Transform t = b2Body_GetTransform(body.bodyId);
    sprite.Draw({ 
        M_2_PX(t.p.x) - ((float)sprite.texture.width / 2), 
        M_2_PX(t.p.y) - ((float)sprite.texture.height / 2)
    });
}

void Bullet::onCollision(PhysicsObject* other){
    ObjectType otherType = other->getType();
    if (otherType == ObjectType::OBJ_TYPE_GROUND || otherType == (isPlayerBullet ? ObjectType::OBJ_TYPE_ENEMY : ObjectType::OBJ_TYPE_PLAYER)) {
        StageDelete(actor_id);
    }
}

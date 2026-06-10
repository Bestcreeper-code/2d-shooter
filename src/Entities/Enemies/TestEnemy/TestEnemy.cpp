#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "Bullets/Bullet.hpp"
#include "Collision/Collisions.hpp"
#include "Entities/HealthBar/HealthBar.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Textures/TextureCache.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "config.hpp"
#include "raylib.h"

#include "main.hpp"
#include <cstdio>


TestEnemy::TestEnemy(Vector2 pos) : healthBar(30, 5){
    speed = 1.0f;
    health= max_health;
    

    sprite.SetTexture(TextureCache::GetTexture(IMG_DIR"Testenemy.png"));


    b2Filter filter;
    filter.categoryBits = COLLISION_LAYER_ENEMY;
    filter.maskBits = 
        COLLISION_LAYER_GROUND | COLLISION_LAYER_PLAYER 
        | COLLISION_LAYER_PLAYER_BULLET;

    
    body = CreateBoxBody(
        gWorld, {PX_2_M(pos.x), PX_2_M(pos.y) },
        PX_2_M(sprite.texture.width)/2,PX_2_M(sprite.texture.height)/2,
        1, 1,0, true, 
        filter);
    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 
    b2Body_SetBullet(body.bodyId, true);//pass through same layer
}

void TestEnemy::Init(){
    b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
}

TestEnemy::~TestEnemy(){
    DestroyBody(body);
    AddScore(1);
}

void TestEnemy::Draw() {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    Texture2D tex = sprite.texture;
    
    sprite.Draw({ 
        M_2_PX(t.p.x) - ((float)tex.width / 2), 
        M_2_PX(t.p.y) - ((float)tex.height / 2)
    });

    healthBar.Draw((b2Vec2){M_2_PX(t.p.x), M_2_PX(t.p.y) - 30}, health/max_health*100);

    

}



void TestEnemy::Update(float deltaTime){

    float curr_speed = (speed * deltaTime)*25;

    b2Vec2 velocity = {0.0f, 0.0f};

    if (time_until_can_shoot <= 0.0f) {
        b2Transform t = b2Body_GetTransform(body.bodyId);

        Vector2 bulletPos = {M_2_PX(t.p.x), M_2_PX(t.p.y) + 10};
        Bullet* bullet = new Bullet(false, bulletPos, 1.0f);
        RegisterActor(bullet);

        time_until_can_shoot = shoot_cooldown;
    }
    
    velocity.y += curr_speed;

    if (time_until_can_shoot > 0.0f) {
        time_until_can_shoot -= deltaTime;
    }
    
    
    b2Body_SetLinearVelocity(body.bodyId, velocity);
    
    
}

void TestEnemy::onCollision(PhysicsObject *other){
    if (other->getType() == ObjectType::OBJ_TYPE_PLAYER_BULLET) {
        health -= ((Bullet*)other)->damage;
        if(health <= 0) {
            StageDelete(actor_id);
        }
    }
}
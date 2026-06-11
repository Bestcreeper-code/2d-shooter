#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "Bullets/Bullet.hpp"
#include "Collision/Collisions.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
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
#include <cstdlib>
#include <unistd.h>


TestEnemy::TestEnemy(Vector2 pos) : healthBar(30, 5){
    speed = 1.0f;
    health= max_health;
    

    sprite.SetTexture(TextureCache::GetTexture(IMG_DIR"Testenemy.png"));


    // b2Filter filter;
    body.collision_filter.categoryBits = COLLISION_LAYER_ENEMY;
    // body.collision_filter.groupIndex = -1;
    body.collision_filter.maskBits = 
        COLLISION_LAYER_PLAYER | COLLISION_LAYER_PLAYER_BULLET;

    
    body = CreateBoxBody(
        gWorld, {PX_2_M(pos.x), PX_2_M(pos.y) },
        PX_2_M(sprite.texture.width)/2,PX_2_M(sprite.texture.height)/2,
        1, 1,0, true, 
        body.collision_filter);
    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 
    // b2Body_SetBullet(body.bodyId, true);//pass through same layer
}

void TestEnemy::Init(){
    b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
}

TestEnemy::~TestEnemy(){
    DestroyBody(body);
    
    if(give_point) AddScore(1);
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
    b2Transform t = b2Body_GetTransform(body.bodyId);

    if (time_until_can_shoot <= 0.0f) {

        Vector2 bulletPos = {M_2_PX(t.p.x), M_2_PX(t.p.y) + 10};
        Bullet* bullet = new Bullet(false, bulletPos, 1.0f);
        RegisterActor(bullet);

        time_until_can_shoot = shoot_cooldown;
    }
    
    velocity.y += curr_speed;

    if (time_until_can_shoot > 0.0f) {
        time_until_can_shoot -= deltaTime;
    }
    

    if(M_2_PX(t.p.y)> WINDOW_HEIGHT + (sprite.texture.height*2)){
        printf("\e[34m oos death\e[0m");
        give_point = false;
        StageDelete(actor_id);
    }
    
    b2Body_SetLinearVelocity(body.bodyId, velocity);
    
    
}

void TestEnemy::onCollision(PhysicsObject *other){
    b2Vec2 p = b2Body_GetPosition(body.bodyId);
    switch (other->getType()) {
    
        case ObjectType::OBJ_TYPE_PLAYER_BULLET: {
            health -= ((Bullet*)other)->damage;
            if(health <= 0) {
                give_point = true;
                if((rand()%10)>7)RegisterActor(new HealthPack(Vector2{M_2_PX(p.x),M_2_PX(p.y)}, rand()%15));
                StageDelete(actor_id);
            }
            break;
        }
        case ObjectType::OBJ_TYPE_PLAYER: {
            give_point = true;
            StageDelete(actor_id);
            break;
        }

        

        default:
            break;
    }
}
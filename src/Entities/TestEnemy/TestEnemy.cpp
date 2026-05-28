#include "Entities/TestEnemy/TestEnemy.hpp"
#include "Bullets/Bullet.hpp"
#include "Collision/Collisions.hpp"
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


TestEnemy::TestEnemy() {
    speed = 2.0f;

    
    sprite.SetTexture(LoadTexture(IMG_DIR"Testenemy.png"));
    
    b2Filter filter;
    filter.categoryBits = COLLISION_LAYER_ENEMY;
    filter.maskBits = COLLISION_LAYER_GROUND | COLLISION_LAYER_PLAYER | COLLISION_LAYER_PLAYER_BULLET;
    
    body = CreateBoxBody(this, gWorld, {2,2}, PX_2_M(sprite.texture.width)/2,PX_2_M(sprite.texture.height)/2, 1, 1,0, true, filter);
    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 
}


void TestEnemy::Draw() {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    Texture2D tex = sprite.texture;
    
    sprite.Draw({ 
        M_2_PX(t.p.x) - ((float)tex.width / 2), 
        M_2_PX(t.p.y) - ((float)tex.height / 2)
    });

    DrawRectangle(M_2_PX(t.p.x) - 20, M_2_PX(t.p.y) - 30, 40, 5, RED);
    DrawRectangle(M_2_PX(t.p.x) - 20, M_2_PX(t.p.y) - 30, (int)(40 * (health / 100.0f)), 5, GREEN);

}



void TestEnemy::Update(){

    float delta_time = GetFrameTime();

    b2Vec2 velocity = {0.0f, 0.0f};

    if (time_until_can_shoot <= 0.0f) {
        b2Transform t = b2Body_GetTransform(body.bodyId);

        b2Vec2 bulletPos = {M_2_PX(t.p.x), M_2_PX(t.p.y) + 10};
        Bullet* bullet = new Bullet(false, bulletPos, 1.0f);
        RegisterObject(bullet);

        time_until_can_shoot = shoot_cooldown;
    }
    // normalized
    if (velocity.x != 0.0f || velocity.y != 0.0f)
    {
        float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity.x = (velocity.x / length) * speed;
        velocity.y = (velocity.y / length) * speed;
    }

    if (time_until_can_shoot > 0.0f) {
        time_until_can_shoot -= delta_time;
    }
    
    
    b2Body_SetLinearVelocity(body.bodyId, velocity);
    
    
}

void TestEnemy::onCollision(PhysicsObject *other){
    if (other->getType() == ObjectType::OBJ_TYPE_PLAYER_BULLET) {
        health -= ((Bullet*)other)->damage;
        if(health <= 0) StageDelete(this);
    }
}
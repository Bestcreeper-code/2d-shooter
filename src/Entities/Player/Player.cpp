#include "Entities/Player/Player.hpp"
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


Player::Player() : healthBar(30.0f, 10.0f) {
    speed = 2.0f;

    max_health = 100.0f;
    health = 100.0f;
    
    sprite.frameDuration = 0.1f;
    sprite.AddFrame(TextureCache::GetTexture(IMG_DIR"Player1.png"));
    sprite.AddFrame(TextureCache::GetTexture(IMG_DIR"Player2.png"));
    sprite.AddFrame(TextureCache::GetTexture(IMG_DIR"Player3.png"));
    
    b2Filter filter;
    filter.categoryBits = COLLISION_LAYER_PLAYER;
    filter.maskBits = COLLISION_LAYER_GROUND | COLLISION_LAYER_ENEMY | COLLISION_LAYER_ENEMY_BULLET
                    | COLLISION_LAYER_PLAYER_BONUS;
    
    body = CreateBoxBody(gWorld, {3,3}, PX_2_M(sprite.GetFrame(0).width)/2,PX_2_M(sprite.GetFrame(0).height)/2, 1, 1,0, true, filter);
    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 
}

Player::~Player() {
    DestroyBody(body);
}


void Player::Draw() {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    Texture2D tex = sprite.GetFrame(0);
    
    sprite.Draw({ 
        M_2_PX(t.p.x) - ((float)tex.width / 2), 
        M_2_PX(t.p.y) - ((float)tex.height / 2)
    });

    healthBar.Draw({M_2_PX(t.p.x), M_2_PX(t.p.y) - 40}, health / max_health * 100);
}

float time_until_can_shoot = 0.0f;

void Player::Update(float deltaTime){
    
    b2Vec2 velocity = {0.0f, 0.0f};
    
    was_hit_this_frame = false;
    

    if (IsKeyDown(GAME_KEY_RIGHT))
        velocity.x += speed;
    if (IsKeyDown(GAME_KEY_LEFT))
        velocity.x -= speed;

    if (IsKeyDown(GAME_KEY_UP))
        velocity.y -= speed;
    if (IsKeyDown(GAME_KEY_DOWN))
        velocity.y += speed;


    if (IsKeyDown(GAME_KEY_FIRE) && time_until_can_shoot <= 0.0f) {
        b2Transform t = b2Body_GetTransform(body.bodyId);

        b2Vec2 bulletPos = {M_2_PX(t.p.x), M_2_PX(t.p.y)-10};
        Bullet* bullet = new Bullet(true, bulletPos, 10.0f);
        RegisterActor(bullet);

        time_until_can_shoot = shoot_cooldown;
    }
    // normalized
    if (velocity.x != 0.0f || velocity.y != 0.0f)
    {
        float length = sqrtf(velocity.x * velocity.x + velocity.y * velocity.y);
        velocity.x = (velocity.x / length) * speed;
        velocity.y = (velocity.y / length) * speed;
    }

    
    
    b2Body_SetLinearVelocity(body.bodyId, velocity);
    
    sprite.Update(deltaTime);
    if (time_until_can_shoot > 0.0f) {
        time_until_can_shoot -= deltaTime;
    }
    // printf("hp: %f\n",health);

    
}

void Player::onCollision(PhysicsObject* other) {
    if (other->pendingDelete)return;
    if (other->getType() == ObjectType::OBJ_TYPE_ENEMY_BULLET) {
        Bullet* bullet = static_cast<Bullet*>(other);
        if (!bullet) return;
        health -= bullet->damage;
        if(health<=0){
            
            StageDelete(actor_id);
        }
        was_hit_this_frame = true;
    }
}
#include "Entities/Player/Player.hpp"
#include "Bullets/Bullet.hpp"
#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Textures/TextureCache.hpp"
#include "Sounds/SoundCache.hpp"
#include "UI/GameOver/GameOver.hpp"
#include "UI/Shop/Shop.hpp"
#include "box2d/box2d.h"
#include "box2d/collision.h"
#include "box2d/math_functions.h"
#include "box2d/types.h"
#include "config.hpp"
#include "macros.hpp"
#include "raylib.h"

#include "main.hpp"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <format>
#include <string>



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


void max_health_buy(ShopItem* item) {
    Player* player = static_cast<Player*>(item->userData);
    item->price = (uint32_t)(player->max_health / 50) * 8; // recalc next time
    if (score < item->price) return;
    score -= item->price;
    player->max_health += 50;
    player->health += 50;
    if (player->health > player->max_health)
        player->health = player->max_health;

    
    
}



void attack_speed_buy(ShopItem* item) {
    Player* player = static_cast<Player*>(item->userData);

    item->price = (uint32_t)(0.3f / player->shoot_cooldown * 10);
    if (score < item->price) return;
    score -= item->price;
    player->shoot_cooldown *= 0.80f;
    if (player->shoot_cooldown < 0.05f)
        player->shoot_cooldown = 0.05f;
}

void Player::Init() {
    
    shop->AddItem("Max Health", "+50 max HP", 8, max_health_buy, player);
        
    shop->AddItem("Attack Speed", "Fire 20%% faster",
        (uint32_t)(0.3f / player->shoot_cooldown * 10),
        attack_speed_buy, player);
    
        b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
    
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

        Vector2 bulletPos = {M_2_PX(t.p.x), M_2_PX(t.p.y)-10};
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

void Player::Die(){
    gamePaused=true;
    gameOverScreen->show = true;
    enemySpawner->spawning = false;
    
    PlaySound(SoundCache::GetSound("res/snd/death_laugh.wav"));

    StageDelete(actor_id);
}

void Player::onCollision(PhysicsObject* other) {
    printf("const char *__restrict format, ...");
    if (other->getType() == ObjectType::OBJ_TYPE_ENEMY_BULLET) {
        Bullet* bullet = static_cast<Bullet*>(other);
        if (!bullet) return;
        health -= bullet->damage;
        if(health<=0){
            
            Die();
        }
        was_hit_this_frame = true;
    }
}


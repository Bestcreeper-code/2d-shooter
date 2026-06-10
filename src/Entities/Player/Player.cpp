#include "Entities/Player/Player.hpp"
#include "Bullets/BigBullet.hpp"
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
#include <unordered_map>



Player::Player() : healthBar(30.0f, 10.0f) {
    
    

    speed = 5.0f;

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
    
    body = CreateBoxBody(
        gWorld, {3,3}, 
        PX_2_M(sprite.GetFrame(0).width)/2,PX_2_M(sprite.GetFrame(0).height)/2,
        1, 1,0, true, 
        filter);
    
    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f; 
    b2Body_SetMassData(body.bodyId, massData); 

    AddWeapon(BULLET_TYPE_STANDARD, 0.2f);
    
    
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



void attack_speed_base_weapon_buy(ShopItem* item) {
    Player* player = static_cast<Player*>(item->userData);

    BulletSpawnerData* s = player->GetWeapon(0);
    if (!s) return;

    item->price = (uint32_t)(0.3f / s->cooldown * 10);
    if (score < item->price) return;
    score -= item->price;

    s->cooldown *= 0.80f;
    if (s->cooldown < 0.05f)
        s->cooldown = 0.05f;
}

static int _big_weapon_id = 0;
static int _big_weapon_damage = 30;
void big_weapon_buy(ShopItem* item) {
    if (score < item->price) return;
    if(!_big_weapon_id)
    {
        _big_weapon_id = player->AddWeapon(BULLET_TYPE_BIG, 2.0f);
        item->price = 20;
        return;
    } 
    item->price *= 1.5f;
    BulletSpawnerData* data = player->GetWeapon(_big_weapon_id);
    data->cooldown *=0.90;
    _big_weapon_damage *= 1.2;
}

void Player::Init() {
    shop->AddItem("Max Health", "+50 max HP", 8, max_health_buy, player);
    shop->AddItem("Big Gun", "Adds a big secondary", 8, big_weapon_buy, player);

    BulletSpawnerData* w = GetWeapon(0);
    shop->AddItem("Attack Speed", "Fire 20% faster",
        w ? (uint32_t)(0.3f / w->cooldown * 10) : 10,
        attack_speed_base_weapon_buy, player);

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

void Player::Update(float deltaTime) {
    b2Vec2 velocity = {0.0f, 0.0f};
    float curr_speed = speed * deltaTime * 25;
    was_hit_this_frame = false;

    if (IsKeyDown(GAME_KEY_RIGHT)) velocity.x += curr_speed;
    if (IsKeyDown(GAME_KEY_LEFT))  velocity.x -= curr_speed;
    if (IsKeyDown(GAME_KEY_UP))    velocity.y -= curr_speed;
    if (IsKeyDown(GAME_KEY_DOWN))  velocity.y += curr_speed;

    if (velocity.x != 0.0f || velocity.y != 0.0f) {
        float len = sqrtf(velocity.x*velocity.x + velocity.y*velocity.y);
        velocity.x = velocity.x / len * curr_speed;
        velocity.y = velocity.y / len * curr_speed;
    }

    b2Transform t = b2Body_GetTransform(body.bodyId);

    for (auto& [id, s] : weapons) {  // was missing structured binding
        s._timer -= deltaTime;

        if (IsKeyDown(GAME_KEY_FIRE) && s._timer <= 0.0f) {
            Vector2 pos = {
                M_2_PX(t.p.x) + s.offset.x,
                M_2_PX(t.p.y) + s.offset.y
            };

            switch (s.type) {
                case BULLET_TYPE_STANDARD:
                    RegisterActor(new Bullet(true, pos, 10.0f));
                    break;  

                case BULLET_TYPE_BIG:
                    RegisterActor(new BigBullet(true, pos, _big_weapon_damage));
                    break; 
                     
                default:
                    throw ":skull:";
            }

            s._timer = s.cooldown;
        }
    }

    b2Body_SetLinearVelocity(body.bodyId, velocity);
    sprite.Update(deltaTime);
}

void Player::Die(){
    gamePaused=true;
    gameOverScreen->show = true;
    enemySpawner->spawning = false;
    
    PlaySound(SoundCache::GetSound("res/snd/death_laugh.wav"));

    StageDelete(actor_id);
}

void Player::onCollision(PhysicsObject* other) {
    
    switch (other->getType()) {
        case ObjectType::OBJ_TYPE_ENEMY_BULLET: {
            Bullet* bullet = static_cast<Bullet*>(other);
            if (!bullet) return;
            health -= bullet->damage;
            if(health<=0){
                
                Die();
            }
            was_hit_this_frame = true;
            break;
        }

        case ObjectType::OBJ_TYPE_ENEMY: {
            health*= 0.2;
            break;
        }
        
        default:
            break;
    }
}


int Player::AddWeapon(BulletType type, float cooldown, Vector2 offset) {
    int id = _nextSpawnerId++;
    weapons[id] = {type, cooldown, offset, 0.0f};
    return id;
}

BulletSpawnerData* Player::GetWeapon(int id) {
    auto it = weapons.find(id);
    if (it == weapons.end()) return nullptr;
    return &it->second;
}

void Player::RemoveWeapon(int id) {
    weapons.erase(id);
}   
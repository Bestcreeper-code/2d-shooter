#include "Strafer.hpp"
#include "Bullets/Bullet.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
#include "Entities/Enemies/EnemySpawner.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Textures/TextureCache.hpp"
#include "config.hpp"
#include "main.hpp"
#include <cstdlib>

StraferEnemy::StraferEnemy(Vector2 startPos, float strafe_y)
    : healthBar(30, 5), strafe_y(strafe_y) {

    health    = max_health;
    areaRight = WINDOW_WIDTH - 10.f;

    sprite.SetTexture(TextureCache::GetTexture(IMG_DIR"Strafer.png"));

    body.collision_filter.categoryBits = COLLISION_LAYER_ENEMY;
    body.collision_filter.maskBits     =
        COLLISION_LAYER_PLAYER | COLLISION_LAYER_PLAYER_BULLET;

    body = CreateBoxBody(
        gWorld, { PX_2_M(startPos.x), PX_2_M(startPos.y) },
        PX_2_M(sprite.texture.width)  / 2,
        PX_2_M(sprite.texture.height) / 2,
        1, 1, 0, true,
        body.collision_filter);

    b2MassData massData = b2Body_GetMassData(body.bodyId);
    massData.rotationalInertia = 1e38f;
    b2Body_SetMassData(body.bodyId, massData);
}

void StraferEnemy::Init() {
    dir = (rand() % 2) ? 1 : -1;
    b2Body_SetUserData(body.bodyId, (void*)(uintptr_t)ActorIdToUint64(this->actor_id));
}

StraferEnemy::~StraferEnemy() {
    DestroyBody(body);
    if (give_point) AddScore(1);
}

void StraferEnemy::Update(float dt) {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    float px = M_2_PX(t.p.x);
    float py = M_2_PX(t.p.y);

    if (!hasEntered) {
        if (py >= 0.f) hasEntered = true;
        else return;
    }

    if (py > WINDOW_HEIGHT + (sprite.texture.height * 2)) {
        printf("\e[34m oos death\e[0m");
        Die(0);
        return;
    }

    b2Vec2 velocity = { 0.f, 0.f };

    switch (phase) {

        case Phase::Drop: {
            velocity.y = PX_2_M(dropSpeed);
            if (py >= strafe_y) {
                phase = Phase::Strafe;
            }
            break;
        }
        case Phase::Strafe: {
            velocity.x = PX_2_M(speed * dir);
        
            if (px >= areaRight)  dir = -1;
            else if (px <= areaLeft) dir =  1;
        
            time_until_can_shoot -= dt;
            if (time_until_can_shoot <= 0.f) {
                time_until_can_shoot = shoot_cooldown;
                RegisterActor(new Bullet(false, { px, py + 10.f }, 1.f));
            }
            break;
        }
    }

    b2Body_SetLinearVelocity(body.bodyId, velocity);
}

void StraferEnemy::Draw() {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    float px = M_2_PX(t.p.x);
    float py = M_2_PX(t.p.y);

    sprite.Draw({
        px - (sprite.texture.width  / 2.f),
        py - (sprite.texture.height / 2.f)
    });

    healthBar.Draw((b2Vec2){ px, py - 30.f }, health / max_health * 100);
}

void StraferEnemy::onCollision(PhysicsObject* other) {
    b2Vec2 p = b2Body_GetPosition(body.bodyId);
    switch (other->getType()) {

        case ObjectType::OBJ_TYPE_PLAYER_BULLET: {
            health -= ((Bullet*)other)->damage;
            if (health <= 0) {
                if ((rand() % 10) > 7)
                    RegisterActor(new HealthPack({ M_2_PX(p.x), M_2_PX(p.y) }, rand() % 15));
                Die(1);
            }
            break;
        }

        case ObjectType::OBJ_TYPE_PLAYER: {
            Die(2);
            break;
        }

        default:
            break;
    }
}

void StraferEnemy::Die(uint8_t points) {
    gEnemySpawner->OnEnemyDied();
    score += points;
    StageDelete(actor_id);
}
#include "HealthPack.hpp"
#include "Bonus.hpp"
#include "Entities/Player/Player.hpp"



void HealthPack::Draw() {
    b2Transform t = b2Body_GetTransform(body.bodyId);
    Bonus::DrawSprite(t.p);
}

void HealthPack::onCollision(PhysicsObject* other) {
    if (other->getType() == ObjectType::OBJ_TYPE_PLAYER) {
        Player* player = static_cast<Player*>(other);
        player->health = std::min(player->health + heal_amount, player->max_health);
        pendingDelete = true;
    }
}
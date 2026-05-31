#pragma once

#include "Entities/Bonuses/Bonus.hpp"
#include "Object/Object.hpp"
#include "config.hpp"

class HealthPack : public Bonus {
public:
    float heal_amount;    

    const ObjectType getType() override {
        return ObjectType::OBJ_TYPE_PLAYER_BONUS;
    }
    


    HealthPack(px_Vec2 position, float healing_amount) : Bonus(position, IMG_DIR"health_pack.png")  {
        this->heal_amount = healing_amount;
    }
    ~HealthPack();

    void Draw()override;
    void Update(float deltaTime) override {}
    void onCollision(PhysicsObject* other) override;
    
};
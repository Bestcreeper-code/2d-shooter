#pragma once

#include "Collision/Collisions.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
class EnemySpawner : public Actor {
        float spawnTimer;
        const float spawnInterval = 5.0f; 
        px_Vec2 spawn_top_left;
        px_Vec2 spawn_bott_right;
    public:
    
        EnemySpawner(px_Vec2 spawn_area_top_left, px_Vec2 spawn_area_bott_right);

        void Update(float deltaTime) override;
    
};
#pragma once

#include "Collision/Collisions.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
class EnemySpawner : public Actor {
        float spawnTimer;
        float spawnInterval = 3.0f; 
        Vector2 spawn_top_left;
        Vector2 spawn_bott_right;
    public:
        bool spawning;

        EnemySpawner(Vector2 spawn_area_top_left, Vector2 spawn_area_bott_right);

        void Init() override {}
        void Update(float deltaTime) override;
    
};
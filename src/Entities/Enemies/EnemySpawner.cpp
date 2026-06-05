#include "EnemySpawner.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Collision/Collisions.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include <cmath>
#include <cstdlib>


EnemySpawner::EnemySpawner(px_Vec2 spawn_area_top_left, px_Vec2 spawn_area_bott_right) 
: spawn_top_left(spawn_area_top_left), spawn_bott_right(spawn_area_bott_right) {

    spawnTimer = 0.0f;
    spawning = true;
}


void EnemySpawner::Update(float deltaTime) {
    spawnTimer += deltaTime;
    if (spawnTimer >= spawnInterval && spawning) {
        spawnTimer = 0.0f;
        
        RegisterActor(new TestEnemy(
                (px_Vec2) {
                    (float)(rand() % (int)std::floor(spawn_bott_right.x - spawn_top_left.x)),
                    (float)(rand() % (int)std::floor(spawn_bott_right.y - spawn_top_left.y)),
                }
            )
        );
        
    }
}
#pragma once


#include "Entities/Enemies/EnemySpawner.hpp"
#include "Entities/Player/Player.hpp"
#include "Object/Object.hpp"
#include "UI/GameOver/GameOver.hpp"
#include <vector>


extern uint32_t score;
extern Player* player;
extern EnemySpawner* enemySpawner;
extern GameOverScreen* gameOverScreen;
extern bool shop_on;
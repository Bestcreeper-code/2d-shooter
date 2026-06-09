#pragma once


#include "Entities/Enemies/EnemySpawner.hpp"
#include "Entities/Player/Player.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Sounds/SoundCache.hpp"
#include "UI/GameOver/GameOver.hpp"
#include "UI/Shop/Shop.hpp"
#include "raylib.h"
#include <vector>


extern uint32_t score;

void AddScore(int amount);

extern Player* player;
extern EnemySpawner* enemySpawner;
extern GameOverScreen* gameOverScreen;
extern Shop* shop;
extern bool shop_on;


enum DrawLayer {
    DL_BG = 0,
    DL_CHARACTERS = 1,

    DL_MENUS_BG,
    DL_MENUS_FG,
    DL_HUD,
    DL_PAUSE_MENU_BG,
    DL_PAUSE_MENU_FG,
    MAX_DRAW_LAYERS
};
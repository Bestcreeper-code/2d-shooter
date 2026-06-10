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


enum DrawLayer {
    DL_BG = 0,
    DL_CHARACTERS = 1,

    DL_MENUS_BG,
    DL_MENUS_FG,
    DL_HUD_BG,
    DL_HUD_FG,
    DL_PAUSE_MENU_BG,
    DL_PAUSE_MENU_FG,
    MAX_DRAW_LAYERS
};

extern uint32_t score;


extern Player* player;
extern EnemySpawner* enemySpawner;
extern GameOverScreen* gameOverScreen;
extern Shop* shop;

extern bool shop_on;
extern bool game_should_end;

void AddScore(int amount);
void EndGame();

Vector2 GetWarpedMousePosition();
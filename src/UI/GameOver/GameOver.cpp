#include "GameOver.hpp"
#include "Entities/Player/Player.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Sounds/SoundCache.hpp"
#include "UI/Button/Button.hpp"
#include "config.hpp"
#include "main.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdint>
#include <cstdlib>


GameOverScreen::GameOverScreen() {
    pausable = false;
    show = false;
    drawing_layer = DL_PAUSE_MENU_BG;

    restartButton = new Button(
        Vector2{(float)WINDOW_WIDTH/2-100,WINDOW_HEIGHT*0.8},
        200,40,
        "RESTART", 30, 
        [](void*){
            restart_request = true;
        }, nullptr,
        YELLOW, GOLD, BLACK, DARKGRAY
    );
    deathSound = new Sound(SoundCache::GetSound("res/snd/death_laugh.wav"));
    
}

GameOverScreen::~GameOverScreen(){
    delete restartButton;
    delete deathSound;
}

void GameOverScreen::Update(float deltaTime) {
    if(!show) return;

    if(!IsSoundPlaying(*deathSound)) restartButton->Update(deltaTime);
}

void GameOverScreen::Draw() {
    if(!show) return;
    if(!played_sound){
        PlaySound(*deathSound);
        player->max_health = -10;
        played_sound = true;
    }


    uint8_t r = (uint8_t)rand()%255;
    uint8_t g = (uint8_t)rand()%255;
    uint8_t b = (uint8_t)rand()%255;
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{r,g,b,255});
    int textSize = MeasureText("Game Over", 32);
    DrawText("Game Over", (WINDOW_WIDTH-textSize)/2, (WINDOW_HEIGHT-16)/2, 32, BLACK);
    
    
    if(!IsSoundPlaying(*deathSound)){
        DrawRectangle(
            (float)WINDOW_WIDTH/2-100,WINDOW_HEIGHT*0.8,
            207,47,
            Color{b,r,g,255}
        );

        restartButton->Draw();
    }
}
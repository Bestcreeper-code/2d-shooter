#include "GameOver.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "config.hpp"
#include "raylib.h"
#include <cstdint>
#include <cstdlib>


GameOverScreen::GameOverScreen() {
    pausable = false;
    layer = MAX_DRAW_LAYERS-3;
}

void GameOverScreen::Update(float deltaTime) {
    
}

void GameOverScreen::Draw() {
    if(!show) return;

    uint8_t r = (uint8_t)rand()%255;
    uint8_t g = (uint8_t)rand()%255;
    uint8_t b = (uint8_t)rand()%255;
    DrawRectangle(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, Color{r,g,b,255});
    int textSize = MeasureText("Game Over", 32);
    DrawText("Game Over", (WINDOW_WIDTH-textSize)/2, (WINDOW_HEIGHT-16)/2, 32, BLACK);
}
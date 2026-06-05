#include "Hud.hpp"
#include "Entities/Player/Player.hpp"
#include "config.hpp"
#include "raylib.h"
#include "main.hpp"
#include <cstdio>

Hud::Hud() {
    pausable = false;
    layer = MAX_DRAW_LAYERS-2;
}

void Hud::Update(float dTime) {

}

void Hud::Draw() {
    DrawRectangle(0, 0, WINDOW_WIDTH, 20, LIGHTGRAY);
    char buff[32];
    sprintf(buff, "Points: %u", score);
    DrawText(buff, 0, 0, 15, BLUE);
}
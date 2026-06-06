#include "Hud.hpp"
#include "Entities/Player/Player.hpp"
#include "UI/Button/Button.hpp"
#include "config.hpp"
#include "raylib.h"
#include "main.hpp"
#include <cstdio>

Hud::Hud() {
    pausable = false;
    drawing_layer = DL_HUD;
}


void Hud::Update(float dTime) {

}

void Hud::Draw() {
    DrawRectangle(0, 0, WINDOW_WIDTH, 20, LIGHTGRAY);
    char buff[32];
    sprintf(buff, "Points: %u", score);
    DrawText(buff, 0, 0, 15, BLUE);
}
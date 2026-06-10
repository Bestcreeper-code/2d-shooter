#include "Hud.hpp"
#include "Entities/Player/Player.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "UI/Button/Button.hpp"
#include "config.hpp"
#include "raylib.h"
#include "main.hpp"
#include <cstddef>
#include <cstdio>



void endGameCallback(void*){
    EndGame();
}
Hud::Hud() {
    pausable = false;
    drawing_layer = DL_HUD_BG;

    Button* endBtn = new Button(Vector2{WINDOW_WIDTH-60,4},50,20,
        "off",20,endGameCallback,nullptr,
    RED, BROWN, WHITE, LIGHTGRAY);
    
    endBtn->drawing_layer = DL_HUD_FG;
    RegisterActor(endBtn);
    
}



void Hud::Update(float dTime) {

}

void Hud::Draw() {
    DrawRectangle(0, 0, WINDOW_WIDTH, 30, LIGHTGRAY);
    char buff[32];
    sprintf(buff, "Points: %u", score);
    DrawText(buff, 0, 0, 30, BLUE);


    DrawRectangle(WINDOW_WIDTH-60,4,55,25, DARKGRAY);
}
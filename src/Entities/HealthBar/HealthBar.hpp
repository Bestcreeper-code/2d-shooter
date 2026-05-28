#pragma once

#include "raylib.h"
#include "box2d/math_functions.h"



class HealthBar {
private:
    float width;
    float height;
public:

    HealthBar(float width, float height) : width(width), height(height) {
        
    }

    void Draw(b2Vec2 position, float healthPercent) {
        Rectangle bg = { position.x - width / 2, position.y - height / 2, width, height };
        Rectangle fg = { position.x - width / 2, position.y - height / 2, width * healthPercent/100, height };

        DrawRectangleRec(bg, DARKGRAY);
        DrawRectangleRec(fg, GREEN);
    }

};
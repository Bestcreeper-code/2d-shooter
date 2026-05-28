#pragma once
#include "raylib.h"
#include <cmath>

struct Sprite {
    Texture2D texture = {};
    Rectangle srcRect = {};
    Vector2   origin  = {};
    float     scale   = 1.0f;
    Color     tint    = WHITE;

    
    void SetTexture(Texture2D tex) {
        texture = tex;
        srcRect = { 0, 0, (float)tex.width, (float)tex.height };
    }

    void Draw(Vector2 position, float rotation = 0.0f) const {
        Rectangle dst = {
            position.x, position.y,
            fabsf(srcRect.width)  * scale,
            fabsf(srcRect.height) * scale
        };
        DrawTexturePro(texture, srcRect, dst, origin, rotation, tint);
    }

    
    void SetFlipH(bool flip) {
        srcRect.width = flip ? -fabsf(srcRect.width) : fabsf(srcRect.width);
    }
};
#include "Button.hpp"
#include "Entities/Player/Player.hpp"
#include "config.hpp"
#include "raylib.h"
#include "main.hpp"
#include <cstdint>
#include <cstdio>
#include <string>

Button::Button(Vector2 pos, float w, float h,
    std::string text,
    int fontSize,
    void (*click_callback)(void*),
    void (*hold_callback)(void*),
    Color color,
    Color pressed_color,
    Color text_color,
    Color text_pressed_color)
{
    pausable = false;
    _click_callback = click_callback;
    _hold_callback = hold_callback;

    btn_color = color;
    btn_pressed_color = pressed_color;

    this->text = text;
    this->fontSize = fontSize;

    this->text_color = text_color;
    this->text_pressed_color = text_pressed_color;

    rect.x = pos.x;
    rect.y = pos.y;
    rect.width = w;
    rect.height = h;
}

void Button::Update(float dTime) {
    Vector2 mouse_pos = GetWarpedMousePosition();
    bool hovering = CheckCollisionPointRec(mouse_pos, rect);

    if (hovering) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            state = 1;
        }

        if (state == 1 && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
            if (_hold_callback) _hold_callback(userData);
        }

        if (state == 1 && IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            if (_click_callback) _click_callback(userData);
            state = 0;
        }
    } else {
        if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) {
            state = 0;
        }
    }
}
void Button::Draw() {
    DrawRectangle((int)rect.x, (int)rect.y, (int)rect.width, (int)rect.height, 
        state ? btn_pressed_color : btn_color);

    int textWidth = MeasureText(text.c_str(), fontSize);
    int textX = rect.x + (rect.width - textWidth) / 2;
    int textY = rect.y + (rect.height - fontSize) / 2;

    Color currentTextColor = state ? text_pressed_color : text_color;

    DrawText(text.c_str(), textX, textY, fontSize, currentTextColor);
}

void Button::SetText(const std::string& new_text) {
    text = new_text;
}
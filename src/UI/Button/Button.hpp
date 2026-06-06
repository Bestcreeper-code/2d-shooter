#pragma once

#include "UI/Ui.hpp"
#include "raylib.h"
#include <cstddef>
#include <cstdint>
#include <string>


class Button : public UiActor {
    private:
        void (*_click_callback)(void* userData);
        void (*_hold_callback)(void* userData);
        Rectangle rect;
        uint8_t state;
    
        Color btn_color;
        Color btn_pressed_color;
    
        Color text_color;
        Color text_pressed_color;
    
        std::string text;
        int fontSize;
    
    public:
        void* userData = nullptr;

        Button(Vector2 pos, float w, float h,
            std::string text = "",
            int fontSize = 15,
            void (*click_callback)(void*) = nullptr,
            void (*hold_callback)(void*) = nullptr,
            Color color = GRAY,
            Color pressed_color = DARKGRAY,
            Color text_color = BLACK,
            Color text_pressed_color = DARKBLUE);
    
        void Update(float deltaTime) override;
        void Draw() override;    
    };
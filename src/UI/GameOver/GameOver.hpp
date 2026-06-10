#pragma once

#include "UI/Button/Button.hpp"
#include "UI/Ui.hpp"
#include "raylib.h"


class GameOverScreen : public UiActor {
private:
    bool played_sound = false;
    Button* restartButton;
    Sound* deathSound;
public:
    GameOverScreen();
    ~GameOverScreen();
    void Update(float deltaTime) override;
    void Draw() override;
};
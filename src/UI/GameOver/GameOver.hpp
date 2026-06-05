#pragma once

#include "UI/Ui.hpp"


class GameOverScreen : public UiActor {
public :
    GameOverScreen();
    void Update(float deltaTime) override;
    void Draw() override;
};
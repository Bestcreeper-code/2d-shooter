#pragma once

#include "UI/Ui.hpp"
class Hud : public UiActor {
public :
    Hud();

    void Update(float deltaTime) override;
    void Draw() override;
};
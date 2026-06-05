#pragma once

#include "UI/Ui.hpp"


class Shop : public UiActor {
public :
    Shop();
    void Update(float deltaTime) override;
    void Draw() override;
};
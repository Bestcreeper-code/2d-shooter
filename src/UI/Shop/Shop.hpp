#pragma once

#include "UI/Button/Button.hpp"
#include "UI/Ui.hpp"
#include <string>
#include <vector>
#include <memory>

struct ShopItem;

typedef void (*ShopCallback)(ShopItem* item);

struct ShopItem {
    std::string name;
    std::string desc;

    uint32_t price;

    void* userData;
    ShopCallback callback;

    bool markedForRemoval = false;
};


class Shop : public UiActor {
private:
    std::vector<std::unique_ptr<ShopItem>> items;
    std::vector<std::unique_ptr<Button>> buttons;

public:
    Shop();

    void AddItem(const std::string& name,
                 const std::string& desc,
                 uint32_t price,
                 ShopCallback cb,
                 void* userData);

    void Update(float deltaTime) override;
    void Draw() override;
};
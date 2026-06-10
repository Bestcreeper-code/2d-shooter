#include "Shop.hpp"
#include "Entities/Player/Player.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "UI/Button/Button.hpp"
#include "config.hpp"
#include "imgui.h"
#include "main.hpp"
#include "raylib.h"
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <memory>
#include <ostream>
#include <string>
#include <thread>

#define SHOP_WIDTH 300
#define SHOP_HEIGHT 200

Shop::Shop() {
    pausable = false;
    drawing_layer = DL_MENUS_BG;
    show = true;
}

void Shop::Update(float deltaTime) {
    if (!show) return;

    for (auto& btn : buttons)
        btn->Update(deltaTime);

    for (size_t i = 0; i < items.size(); ) {
        if (items[i]->markedForRemoval) {
            items.erase(items.begin() + i);
            buttons.erase(buttons.begin() + i);
        } else {
            buttons[i]->SetText(
                items[i]->name + '(' + std::to_string(items[i]->price) +")"
            );
            
            buttons[i]->text_color = items[i]->price>score?
                    RED : GREEN;
            buttons[i]->text_pressed_color = items[i]->price>score?
                    MAROON : DARKGREEN;

            
            ++i;
        }
    }
}

static void ShopButtonClick(void* ptr)
{
    ShopItem* item = static_cast<ShopItem*>(ptr);

    item->callback(item);
}

void Shop::AddItem(const std::string& name,
                   const std::string& desc,
                   uint32_t price,
                   ShopCallback cb,
                   void* userData)
{
    auto item = std::make_unique<ShopItem>();
    item->name  = name;
    item->desc  = desc;
    item->price = price;
    item->callback = cb;
    item->userData = userData;

    size_t index = items.size();
    items.push_back(std::move(item));
    ShopItem* rawItem = items[index].get();

    float panelW = WINDOW_WIDTH  * 0.6f;
    float panelH = WINDOW_HEIGHT * 0.6f;
    float panelX = (WINDOW_WIDTH  - panelW) / 2.0f;
    float panelY = (WINDOW_HEIGHT - panelH) / 2.0f;

    float btnW = panelW - 40.0f;
    float x    = panelX + 20.0f;
    float y    = panelY + 50.0f + index * 70.0f;   // 50px clears the title

    buttons.push_back(std::make_unique<Button>(
        Vector2{ x, y },
        btnW, 50,
        rawItem->name,
        25,
        ShopButtonClick,
        nullptr,
        BLUE, DARKBLUE, GREEN, DARKGREEN
    ));

    buttons.back()->userData = rawItem;
}

void Shop::Draw() {
    if (!show) return;

    float panelW = WINDOW_WIDTH  * 0.6f;
    float panelH = WINDOW_HEIGHT * 0.6f;
    float panelX = (WINDOW_WIDTH  - panelW) / 2.0f;
    float panelY = (WINDOW_HEIGHT - panelH) / 2.0f;

    DrawRectangle((int)panelX, (int)panelY, (int)panelW, (int)panelH, Fade(BLACK, 0.85f));
    DrawRectangleLines((int)panelX, (int)panelY, (int)panelW, (int)panelH, WHITE);

    DrawText("Shop", (int)(panelX + 16), (int)(panelY + 12), 20, WHITE);

    for (auto& btn : buttons)
        btn->Draw();
}
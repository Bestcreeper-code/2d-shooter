#include "Shop.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "config.hpp"
#include "imgui.h"
#include "main.hpp"
#include <cstdint>
#include <cstdlib>


Shop::Shop() {
    pausable = false;
    
}

void Shop::Update(float deltaTime) {
    
}

void Shop::Draw() {

    if(ImGui::Begin("Shop", &shop_on, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {

        if (ImGui::CollapsingHeader("Health", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::MenuItem("Debug Draw",);
        }

        ImGui::End();
    }
}
#include "Collision/Collisions.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
#include "Entities/Enemies/EnemySpawner.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "osdep.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Platforms/Wall/Wall.hpp"
#include "Entities/Player/Player.hpp"
#include "box2d/math_functions.h"
#include "config.h"
#include "config.hpp"
#include "imgui_internal.h"
#include "raylib.h"
#include "Debug/Debug.hpp"
#include "imgui.h"

#include "raylib.h"
#include "box2d/box2d.h"
#include "rlImGui.h"
#include <cstdint>



bool debug_on = false;
bool imgui_on = true;

uint32_t score;

Player* player;

int main(int argc, char** argv)
{

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    
    InitAudioDevice();

    rlImGuiSetup(true);



    
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 0.0f};
    gWorld = b2CreateWorld(&worldDef);

    B2DebugDraw_Init();
    
    RegisterActor(new Wall({0,-32}, WINDOW_WIDTH, 33));
    RegisterActor(new Wall({-32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({WINDOW_WIDTH+32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({0,WINDOW_HEIGHT+32}, WINDOW_WIDTH, 33));
    
    

    player = new Player();
    RegisterActor(player);

    RegisterActor(new HealthPack({200,200}, 25.0f));
    
    RegisterActor(new EnemySpawner((px_Vec2){10,10},(px_Vec2){WINDOW_WIDTH - 10,70}));

    ProcessStagedActions();


    SetTargetFPS(60);

    

    while (!WindowShouldClose()) {

        if (IsKeyPressed(GAME_KEY_DEBUG_TOGGLE)) {
            debug_on = !debug_on;
        } else if (IsKeyPressed(GAME_KEY_IMGUI_TOGGLE)) {
            imgui_on = !imgui_on;
        }
        
        b2World_Step(gWorld, 1.0f / 60.0f, 4);
        ProcessCollisions();
        


        BeginDrawing();
        ClearBackground(BLACK);
        rlImGuiBegin();

        
        if (ImGui::Begin("Debug", &imgui_on, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            ImGui::Text("Score: %u",score);
            if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::MenuItem("Debug Draw", "G", &debug_on);
                ImGui::SliderFloat("Player Health", &player->health, 0.0f, 100.0f);
                ImGui::SliderFloat("Player Attack Speed", &player->shoot_cooldown, 0.0f, 2.0f);
            }            

            if (ImGui::CollapsingHeader("Object Manager"))
            {
                ImGui::Text("Total Actors Slots: %zu", actorSlots.size());
                ImGui::Text("Used Actor List Space:");
                
                ImGui::ProgressBar((float)(GetActorCount() / (float)actorSlots.capacity()), ImVec2(0.0f, 0.0f));
                ImGui::Text("(%u / %zu)", GetActorCount(), actorSlots.capacity());
                
            }   
            if (ImGui::CollapsingHeader("Performances"))
            {
                ImGui::Indent(20.0f);
                if (ImGui::CollapsingHeader("System"))
                {
                    ImGui::Text("FPS: %.1f", (float)GetFPS());
                    ImGui::Text("Frame Time: %.3f ms", (float)GetFrameTime() * 1000);
                    
                    ImGui::Text("Used Memory:");
                    ImGui::Text("Game Allocs: %.2f MB", GetAllocMemUsage() / (1024.0f * 1024.0f));
                    ImGui::Text("Process: %.2f MB", GetTotalProcessMemUsage() / (1024.0f * 1024.0f));
                }
                if (ImGui::CollapsingHeader("Box2D"))
                {
                    b2Counters b2Counts = b2World_GetCounters(gWorld);
                    ImGui::Text("Bodies: %u", b2Counts.bodyCount);
                }
                ImGui::Unindent();

                
            
            }
        }
        ImGui::End();
        

        
        UpdateAll();
        
        if(debug_on)B2DebugDraw_Draw();
        


        rlImGuiEnd();
        EndDrawing();
                
        
        ProcessStagedActions();
    }

    DeleteAllActors();
    
    b2DestroyWorld(gWorld);
    
    rlImGuiShutdown();

    CloseWindow();
    return 0;
}
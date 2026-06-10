#include "main.hpp"
#include "Collision/Collisions.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
#include "Entities/Enemies/EnemySpawner.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "UI/GameOver/GameOver.hpp"
#include "UI/Hud/Hud.hpp"
#include "UI/Shop/Shop.hpp"
#include "macros.hpp"
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
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <random>


//debug
bool debug_on = false;
bool imgui_on = true;


int fps_cap = 60;
float crt_warp = 1.0f;

bool game_should_end = false;
bool restart_request = false;

uint32_t score;

Player* player;
EnemySpawner* enemySpawner;
GameOverScreen* gameOverScreen;
Shop* shop;

float game_rect_x, game_rect_y, game_rect_w, game_rect_h;

void RenderImgui() {
    if(!imgui_on)return;
    rlImGuiBegin();
        if (ImGui::Begin("Debug", &imgui_on, ImGuiWindowFlags_AlwaysVerticalScrollbar)) {
            ImGui::SliderInt("Score",(int*)&score,0,100);
            if (ImGui::CollapsingHeader("Actions", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::MenuItem("Debug Draw", "G", &debug_on);
                
                if (ImGui::Button("Pause")) Pause();
                ImGui::SameLine();
                if (ImGui::Button("Unpause")) ForceUnPause();

                if(ImGui::SliderInt("FPS Cap", &fps_cap,1,500)){
                    SetTargetFPS(fps_cap);
                }
                if(ImGui::SliderFloat("Curve", &crt_warp,0,25)){
                    SetTargetFPS(fps_cap);
                }

                if(player){
                    ImGui::SliderFloat("Player Health", &player->health, 0.0f, 100.0f);
                    ImGui::SliderFloat("Player Attack Speed", &player->GetWeapon(0)->cooldown, 0.0f, 2.0f);
                }
            }            

            if (ImGui::CollapsingHeader("Object Manager"))
            {
                ImGui::Text("Total Actors Slots: %zu", actorSlots.size());
                ImGui::Text("Used Actor List Space:");

                if(ImGui::MenuItem("Dump Actors", "", nullptr)) {
                    DumpActors();
                }

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
    rlImGuiEnd();
}


int main(int argc, char** argv)
{

    srand(time(NULL));

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Game");
    InitAudioDevice();
    SetExitKey(KEY_NULL);

    rlImGuiSetup(true);



    
    // game tex rect 
    float scale = 0.85f;
    float w = WINDOW_WIDTH * scale;
    float h = WINDOW_HEIGHT * scale;
    float x = (WINDOW_WIDTH - w) * 0.5f;
    float y = (WINDOW_HEIGHT - h) * 0.5f;
    game_rect_w = WINDOW_WIDTH  * scale;
    game_rect_h = WINDOW_HEIGHT * scale;
    game_rect_x = (WINDOW_WIDTH  - game_rect_w) * 0.5f;
    game_rect_y = (WINDOW_HEIGHT - game_rect_h) * 0.5f;

    //shaders
    Shader crt_curve_shader = LoadShader(0, "res/shaders/crt_curve_shader.glsl");
    Shader curve_shader = LoadShader(0, "res/shaders/curve_shader.glsl");
    RenderTexture2D gameRenderTexture = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    RenderTexture2D outlineRenderTexture = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 0.0f};
    worldDef.enableSleep = false;
    gWorld = b2CreateWorld(&worldDef);

    
    DebugDraw_Init();

GAME_START:
    gamePaused = 0;
    score = 0;
    game_should_end = false;

//world   
    RegisterActor(new Wall({0,0}, WINDOW_WIDTH, 33));
    RegisterActor(new Wall({-32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({WINDOW_WIDTH+32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({0,WINDOW_HEIGHT+32}, WINDOW_WIDTH, 33));
    
//UI
    gameOverScreen = new GameOverScreen();
    RegisterActor(gameOverScreen);

    RegisterActor(new Hud);
    
    shop = new Shop;
    RegisterActor(shop);


//entities
    player = new Player();
    RegisterActor(player);
       
    enemySpawner = new EnemySpawner((Vector2){10,10},(Vector2){WINDOW_WIDTH - 10,70});
    RegisterActor(enemySpawner);

    RegisterActor(new HealthPack({200,200}, 25.0f));


    ProcessStagedActions();

    SetTargetFPS(fps_cap);
    

    BeginTextureMode(outlineRenderTexture);
    ClearBackground(DARKGRAY);
    EndTextureMode();

    while (!WindowShouldClose() && !game_should_end) {
        if (restart_request) {
            restart_request = false;
            DeleteAllActors();
            goto GAME_START;
        }

        if (IsKeyPressed(GAME_KEY_DEBUG_TOGGLE)) {
            debug_on = !debug_on;
        } else if (IsKeyPressed(GAME_KEY_IMGUI_TOGGLE)) {
            imgui_on = !imgui_on;
        } else if (IsKeyPressed(KEY_P)) {
            gamePaused = !gamePaused;
        } else if (IsKeyPressed(KEY_O)) {
            if(shop->show){ 
                UnPause();
                shop->show = false;
            } else {
                Pause();
                shop->show = true;
            }
        }
        
        if (!gamePaused){
            b2World_Step(gWorld, 1.0f / 60.0f, 4);
            ProcessCollisions();
        }
        

        //render start
        BeginDrawing();
        ClearBackground(BLACK);

        
        // render Game to texture
        BeginTextureMode(gameRenderTexture);
        ClearBackground(BLUE);
        

        //updage all 
        UpdateAll();
        

        //debug outlines
        if(debug_on)B2DebugDraw_Draw();
        EndTextureMode();
        
        // config shader
        int warpLoc = GetShaderLocation(crt_curve_shader, "warpStrength");
        SetShaderValue(crt_curve_shader, warpLoc, &crt_warp, SHADER_UNIFORM_FLOAT);
        
        int warpLoc2 = GetShaderLocation(curve_shader, "warpStrength");
        SetShaderValue(curve_shader, warpLoc2, &crt_warp, SHADER_UNIFORM_FLOAT);
        


        

        // outline rect
        float padding = 22.0f;

        BeginShaderMode(curve_shader);
        DrawTexturePro(
            outlineRenderTexture.texture,
            (Rectangle){0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT},
            (Rectangle){x - padding, y - padding, w + padding * 2, h + padding * 2},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        EndShaderMode();

        // draw the game texture with crt+curve shader
        BeginShaderMode(crt_curve_shader);
        DrawTexturePro(
            gameRenderTexture.texture,
            (Rectangle){0, 0, WINDOW_WIDTH, -WINDOW_HEIGHT},
            (Rectangle){x, y, w, h},
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        EndShaderMode();
        
        // imgui
        RenderImgui();
        
        EndDrawing();

        ProcessStagedActions();
    }

    DeleteAllActors();
    
    b2DestroyWorld(gWorld);
    
    rlImGuiShutdown();

    CloseWindow();
      return 0;
}

void AddScore(int amount) {
    score+=amount;
    PlaySound(SoundCache::GetSound("res/snd/point.wav"));
}

void EndGame() {
    game_should_end = true;
}

Vector2 GetWarpedMousePosition() {
    Vector2 m = GetMousePosition();
    float u = (m.x - game_rect_x) / game_rect_w * 2.0f - 1.0f;
    float v = (m.y - game_rect_y) / game_rect_h * 2.0f - 1.0f;
    float k = crt_warp * 0.1f;
    float r2 = u*u + v*v;
    u *= 1.0f + k * r2;
    v *= 1.0f + k * r2;
    return { (u * 0.5f + 0.5f) * WINDOW_WIDTH, (v * 0.5f + 0.5f) * WINDOW_HEIGHT };
}
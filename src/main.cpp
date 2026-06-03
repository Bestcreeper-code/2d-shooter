#include "Collision/Collisions.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
#include "Entities/Enemies/EnemySpawner.hpp"
#include "Entities/Enemies/TestEnemy/TestEnemy.hpp"
#include "Object/Object.hpp"
#include "ObjectMgr/ObjectMgr.hpp"
#include "Platforms/Wall/Wall.hpp"
#include "Entities/Player/Player.hpp"
#include "box2d/math_functions.h"
#include "config.h"
#include "config.hpp"
#include "raylib.h"
#include "Debug/Debug.hpp"




#include "raylib.h"
#include "box2d/box2d.h"
#include <algorithm>

#include <vector>



int main(int argc, char** argv)
{

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib + Box2D test");
    
    InitAudioDevice();

    
    b2WorldDef worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 0.0f};
    gWorld = b2CreateWorld(&worldDef);

    B2DebugDraw_Init();
    
    RegisterActor(new Wall({0,-32}, WINDOW_WIDTH, 33));
    RegisterActor(new Wall({-32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({WINDOW_WIDTH+32,0}, 33,WINDOW_HEIGHT));
    RegisterActor(new Wall({0,WINDOW_HEIGHT}, WINDOW_WIDTH, 33));
    
    
    RegisterActor(new TestEnemy((px_Vec2){300,30}));

    RegisterActor(new Player());

    RegisterActor(new HealthPack({200,200}, 25.0f));
    
    RegisterActor(new EnemySpawner((px_Vec2){10,10},(px_Vec2){WINDOW_WIDTH - 10,70}));

    ProcessStagedActions();


    SetTargetFPS(60);

    bool debug_on;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(GAME_KEY_DEBUG_TOGGLE)) {
            debug_on = !debug_on;
        }
        
        b2World_Step(gWorld, 1.0f / 60.0f, 4);
        ProcessCollisions();
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        UpdateAll();
        
        if(debug_on)B2DebugDraw_Draw();
        
        EndDrawing();
                
        
        ProcessStagedActions();
    }

    DeleteAllActors();
    
    b2DestroyWorld(gWorld);

    CloseWindow();
    return 0;
}
#include "Collision/Collisions.hpp"
#include "Entities/Bonuses/HealthPack.hpp"
#include "Entities/TestEnemy/TestEnemy.hpp"
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
    worldDef.gravity = (b2Vec2){0.0f, 9.8f};
    gWorld = b2CreateWorld(&worldDef);

    B2DebugDraw_Init();
    
    physicsObjects.push_back(new Wall({0,-32}, WINDOW_WIDTH, 33));
    physicsObjects.push_back(new Wall({-32,0}, 33,WINDOW_HEIGHT));
    physicsObjects.push_back(new Wall({WINDOW_WIDTH+32,0}, 33,WINDOW_HEIGHT));
    physicsObjects.push_back(new Wall({0,WINDOW_HEIGHT}, WINDOW_WIDTH, 33));
    
    
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());
    physicsObjects.push_back(new TestEnemy());

    physicsObjects.push_back(new Player());

    physicsObjects.push_back(new HealthPack({200,200}, 25.0f));
    



    SetTargetFPS(60);

    bool debug_on;

    while (!WindowShouldClose()) {

        if (IsKeyPressed(GAME_KEY_DEBUG_TOGGLE)) {
            debug_on = !debug_on;
        }

        b2World_Step(gWorld, 1.0f / 60.0f, 4);
    
        ProcessCollisions();
        RemoveStaged();
    
        BeginDrawing();
        ClearBackground(BLACK);

        UpdateAll();

        if(debug_on)B2DebugDraw_Draw();

        EndDrawing();
    }

    
    b2DestroyWorld(gWorld);

    CloseWindow();
    return 0;
}
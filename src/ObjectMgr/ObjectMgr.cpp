#include "ObjectMgr/ObjectMgr.hpp"
#include "Object/Object.hpp"
#include <algorithm>


// main.cpp or ObjectManager.cpp

std::vector<Actor*> gameActors;

void RegisterActor(Actor* actor) {
    gameActors.push_back(actor);
}

void UpdateAll() {
    float deltaTime = GetFrameTime();

    size_t count = gameActors.size();

    for (size_t i = 0; i < count; i++) {
        Actor* actor = gameActors[i];
        if (!actor) continue;
        if (actor->pendingDelete) continue;

        actor->Update(deltaTime);
        actor->Draw();
    }
}

void StageDelete(Actor* actor) {
    if (actor->pendingDelete) return;
    actor->pendingDelete = true;
    
}

void RemoveStaged() {
    gameActors.erase(
        std::remove_if(gameActors.begin(), gameActors.end(),
            [](Actor* actor) {
                if (actor->pendingDelete) { delete actor; return true; }
                return false;
            }),
        gameActors.end()
    );
}

void DeleteAllActors() {
    for(Actor* actor : gameActors){
        delete actor;
    }
}
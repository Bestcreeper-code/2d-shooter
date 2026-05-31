#pragma once
#include "Object/Object.hpp"
#include <vector>


extern std::vector<Actor*> gameActors;

void RegisterActor(Actor* obj);
void UpdateAll();
void StageDelete(Actor* obj);
void RemoveStaged();
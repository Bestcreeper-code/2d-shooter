#pragma once

#include <cstdint>


struct ActorId {
    uint32_t index;
    uint32_t generation;
};

#include "Object/Object.hpp"
#include <vector>




ActorId* RegisterActor(Actor* obj);
Actor* GetActor(ActorId* id);
void UpdateAll();
void StageDelete(ActorId* id);
void ProcessStagedActions();

void DeleteAllActors();

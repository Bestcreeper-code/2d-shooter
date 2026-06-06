#pragma once

#include <cstring>
#include <stdint.h>


struct ActorId {
    uint32_t index;
    uint32_t generation;
} __attribute__((packed));

static inline uint64_t ActorIdToUint64(ActorId id) {
    return ((uint64_t)id.generation << 32) | id.index;
}

static inline ActorId Uint64ToActorId(uint64_t value) {
    ActorId result;
    result.index = (uint32_t)(value & 0xFFFFFFFFu);
    result.generation = (uint32_t)(value >> 32);
    return result;
}



#include "Object/Object.hpp"
#include <vector>



struct Slot {
    Actor* ptr;
    uint32_t generation;
};

struct PendingAdd {
    Actor*   ptr;
    ActorId* id;
};

extern bool gamePaused;

extern std::vector<Slot>       actorSlots;
extern std::vector<uint32_t>   freeList;
extern std::vector<PendingAdd> pendingAdds;

void RegisterActor(Actor* obj);
Actor* GetActor(ActorId id);
void UpdateAll();
void StageDelete(ActorId id);
void ProcessStagedActions();

void DeleteAllActors();

uint32_t GetActorCount();
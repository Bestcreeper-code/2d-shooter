#pragma once

#include <cstring>
#include <stdint.h>
#include <vector>

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




struct Slot {
    Actor* ptr;
    uint32_t generation;
};

struct PendingAdd {
    Actor*   ptr;
    ActorId* id;
};

extern uint8_t gamePaused;

extern std::vector<Slot>       actorSlots;
extern std::vector<uint32_t>   freeList;
extern std::vector<PendingAdd> pendingAdds;

void RegisterActor(Actor* obj);
Actor* GetActor(ActorId id);
void UpdateAll();

void DrawAllGui();

void StageDelete(ActorId id);
void ProcessStagedActions();

void DeleteAllActors();

void DumpActors();

uint32_t GetActorCount();

static inline const char* ObjectTypeName(ObjectType t) {
    switch(t) {
        case OBJ_TYPE_PLAYER:        return "PLAYER";
        case OBJ_TYPE_PLAYER_BULLET: return "PLAYER_BULLET";
        case OBJ_TYPE_ENEMY:         return "ENEMY";
        case OBJ_TYPE_ENEMY_BULLET:  return "ENEMY_BULLET";
        case OBJ_TYPE_PLAYER_BONUS:  return "PLAYER_BONUS";
        case OBJ_TYPE_GROUND:        return "GROUND";
        case OBJ_TYPE_INVALID:       return "INVALID";
        default:                     return "UNKNOWN";
    }
}

static inline void Pause() {gamePaused++;}
static inline void UnPause() {if(gamePaused)gamePaused--;}
static inline void ForceUnPause() {gamePaused=0;}
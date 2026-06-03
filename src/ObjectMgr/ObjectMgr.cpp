#include "ObjectMgr/ObjectMgr.hpp"
#include "Object/Object.hpp"
#include "config.hpp"
#include <cstdio>
#include <climits>



struct Slot {
    Actor* ptr;
    uint32_t generation;
};

struct PendingAdd {
    Actor*   ptr;
    ActorId* id;
};

std::vector<Slot>       actorSlots = {};
std::vector<uint32_t>   freeList   = {};
std::vector<PendingAdd> pendingAdds;

ActorId* RegisterActor(Actor* actor) {
    ActorId* newId  = new ActorId{UINT32_MAX, 0};
    actor->actor_id = newId;
    pendingAdds.push_back({actor, newId});
    return newId;
}

Actor* GetActor(ActorId* id) {
    if (id->index >= actorSlots.size()) {
        return nullptr;
    }

    const Slot& slot = actorSlots[id->index];

    if (slot.generation != id->generation) {
        return nullptr;
    }

    return slot.ptr;
}

void UpdateAll() {
    float dt = GetFrameTime();

    for (size_t i = 0; i < actorSlots.size(); i++) {
        Slot& slot = actorSlots[i];

        if (!slot.ptr) continue;
        if (slot.ptr->pendingDelete) continue;

        slot.ptr->Update(dt);
        slot.ptr->Draw();
    }
}

void StageDelete(ActorId* id) {
    Actor* actor = GetActor(id);
    if (!actor) return;
    actor->pendingDelete = true;
}

void RegisterStaged() {
    for (auto& p : pendingAdds) {
        uint32_t index;

        if (!freeList.empty()) {
            index = freeList.back();
            freeList.pop_back();
        } else {
            index = actorSlots.size();
            actorSlots.push_back({nullptr, 0});
        }

        uint32_t gen = actorSlots[index].generation;


        *p.id = {index, gen};

        actorSlots[index] = {p.ptr, gen};
    }

    pendingAdds.clear();
}

void RemoveStaged() {
    for (uint32_t i = 0; i < actorSlots.size(); i++) {
        Slot& slot = actorSlots[i];

        if (!slot.ptr) continue;
        if (!slot.ptr->pendingDelete) continue;

        delete slot.ptr->actor_id;
        delete slot.ptr;
        slot.ptr = nullptr;
        

        slot.generation++;

        freeList.push_back(i);
    }
}

void DeleteAllActors() {
    for (Slot& slot : actorSlots) {
        if (slot.ptr) {
            delete slot.ptr->actor_id;
            delete slot.ptr;
            slot.ptr = nullptr;
        }
    }

    actorSlots.clear();
    freeList.clear();
    pendingAdds.clear();
}

void ProcessStagedActions() {
    RemoveStaged();
    RegisterStaged();
}
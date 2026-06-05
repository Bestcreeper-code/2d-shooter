#include "ObjectMgr/ObjectMgr.hpp"
#include "Object/Object.hpp"
#include "config.hpp"
#include <cstdint>
#include <cstdio>
#include <climits>





bool gamePaused = false;

std::vector<Slot>       actorSlots = {};
std::vector<uint32_t>   freeList   = {};
std::vector<PendingAdd> pendingAdds;

void RegisterActor(Actor* actor) {
    
    actor->actor_id = {UINT32_MAX, UINT32_MAX};
    pendingAdds.push_back({actor, &actor->actor_id});

#ifdef DEBUG_BUILD
    printf("Registered actor at %p\n", (void*)actor);
#endif
    return;
}

Actor* GetActor(ActorId id) {
    if (id.index >= actorSlots.size()) {
        return nullptr;
    }

    const Slot& slot = actorSlots[id.index];

    if (slot.generation != id.generation) {
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

        if (!(slot.ptr->pausable && gamePaused))
            slot.ptr->Update(dt);
    }

    for (uint8_t l = 0; l < MAX_DRAW_LAYERS; l++) {
        for (size_t i = 0; i < actorSlots.size(); i++) {
            Slot& slot = actorSlots[i];

            if (!slot.ptr) continue;
            if (slot.ptr->pendingDelete) continue;
            if (slot.ptr->layer != l) continue;

            slot.ptr->Draw();
        }
    }
}
void StageDelete(ActorId id) {
    Actor* actor = GetActor(id);
    if (!actor) return;
    actor->pendingDelete = true;

#ifdef DEBUG_BUILD
    printf("Staged deletion for actor at %p\n", (void*)actor);
#endif
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
        p.ptr->actor_id = *p.id;
        p.ptr->Init();

#ifdef DEBUG_BUILD
        printf("Added actor at %p to slot %u (generation %u)\n", (void*)p.ptr, index, gen);
#endif

    }

    pendingAdds.clear();
}

void RemoveStaged() {
    for (uint32_t i = 0; i < actorSlots.size(); i++) {
        Slot& slot = actorSlots[i];

        if (!slot.ptr) continue;
        if (!slot.ptr->pendingDelete) continue;

        delete slot.ptr;
        slot.ptr = nullptr;
        

        slot.generation++;

        freeList.push_back(i);

#ifdef DEBUG_BUILD
        printf("Deleted actor from slot %u, new generation %u\n", i, slot.generation);
#endif
    }
}

void DeleteAllActors() {
    for (Slot& slot : actorSlots) {
        if (slot.ptr) {
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

uint32_t GetActorCount() {
    return actorSlots.size() - freeList.size();
}
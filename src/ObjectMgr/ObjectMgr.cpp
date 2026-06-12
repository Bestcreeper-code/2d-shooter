#include "ObjectMgr/ObjectMgr.hpp"
#include "Object/Object.hpp"
#include "config.hpp"
#include "helpers.hpp"
#include "main.hpp"
#include "raylib.h"
#include <cstdint>
#include <cstdio>
#include <climits>





uint8_t gamePaused = false;

std::vector<Slot>       actorSlots = {};
std::vector<uint32_t>   freeList   = {};
std::vector<PendingAdd> pendingAdds;

void RegisterActor(Actor* actor) {
    
    actor->actor_id = {UINT32_MAX, UINT32_MAX};
    pendingAdds.push_back({actor, &actor->actor_id});

#ifdef DEBUG_OBJMGR
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
            if (slot.ptr->drawing_layer != l) continue;

            slot.ptr->Draw();
        }
    }
}



void StageDelete(ActorId id) {
    Actor* actor = GetActor(id);
    if (!actor) return;
    actor->pendingDelete = true;

    PhysicsObject* phys = dynamic_cast<PhysicsObject*>(actor);
    verbose_errf("StageDelete: {%u,%u} %p (%s)",
        id.index, id.generation, (void*)actor,
        phys ? ObjectTypeName(phys->getType()) : "UI");

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
        p.ptr->PreInit();
        p.ptr->Init();

#ifdef DEBUG_OBJMGR
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

        PhysicsObject* phys = dynamic_cast<PhysicsObject*>(slot.ptr);
        verbose_errf("RemoveStaged: slot %u gen %u %p (%s)",
                i, slot.generation, (void*)slot.ptr,
                phys ? ObjectTypeName(phys->getType()) : "UI/non physicsobj");


        delete slot.ptr;
        slot.ptr = nullptr;
        

        slot.generation++;

        freeList.push_back(i);

        
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



void DumpActors() {
    printf("\e[33m-----ACTOR DUMP (%u actors)\e[0m\n", GetActorCount());
    for (size_t i = 0; i < actorSlots.size(); i++) {
        const Slot& slot = actorSlots[i];
        if (!slot.ptr) {
            printf("  slot %zu: [empty, gen=%u]\n", i, slot.generation);
            continue;
        }

        PhysicsObject* phys = dynamic_cast<PhysicsObject*>(slot.ptr);
        if (phys) {
            uint64_t storedUserData = (uint64_t)b2Body_GetUserData(phys->body.bodyId);
            ActorId storedId = Uint64ToActorId(storedUserData);
            bool mismatch = storedId.index      != slot.ptr->actor_id.index ||
                            storedId.generation != slot.ptr->actor_id.generation;

            printf("  slot %zu gen %u | ptr=%p | type=%-14s | actor_id={%u,%u}"
                   " | b2body.index=%u | body_userdata={%u,%u}%s\n",
                i, slot.generation,
                (void*)slot.ptr,
                
                ObjectTypeName(phys->getType()),
                
                slot.ptr->actor_id.index, slot.ptr->actor_id.generation,
                
                phys->body.bodyId.index1,
                storedId.index, storedId.generation,
                mismatch ? "  \e[31m<<< MISMATCH\e[0m" : "");
        } else {
            printf("  slot %zu gen %u | ptr=%p | type=UI/non PhysicsObject"
                   " | actor_id={%u,%u}\n",
                i, slot.generation,
                (void*)slot.ptr,
                slot.ptr->actor_id.index, slot.ptr->actor_id.generation);
        }
    }
    printf("\e[33m--------END OF DUMP[0m\n");
}
#include "ObjectMgr/ObjectMgr.hpp"
#include <algorithm>


// main.cpp or ObjectManager.cpp

std::vector<PhysicsObject*> physicsObjects;

void RegisterObject(PhysicsObject* obj) {
    physicsObjects.push_back(obj);
}

void UpdateAll() {
    for (PhysicsObject* obj : physicsObjects) {
        if (!obj)continue;
        if (!obj->pendingDelete) {
            obj->Update();
        }
        if (!obj->pendingDelete) {
            obj->Draw();
        }
    }
}

void StageDelete(PhysicsObject* obj) {
    if (obj->pendingDelete) return;
    obj->pendingDelete = true;
    b2Body_SetUserData(obj->body.bodyId, nullptr);
    b2DestroyBody(obj->body.bodyId);
}

void RemoveStaged() {
    physicsObjects.erase(
        std::remove_if(physicsObjects.begin(), physicsObjects.end(),
            [](PhysicsObject* obj) {
                if (obj->pendingDelete) { delete obj; return true; }
                return false;
            }),
        physicsObjects.end()
    );
}
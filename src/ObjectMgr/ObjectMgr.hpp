#pragma once
#include "Object/Object.hpp"
#include <vector>


extern std::vector<PhysicsObject*> physicsObjects;

void RegisterObject(PhysicsObject* obj);
void UpdateAll();
void StageDelete(PhysicsObject* obj);
void RemoveStaged();
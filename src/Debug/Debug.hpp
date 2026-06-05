#pragma once

#include "box2d/box2d.h"
#include "raylib.h"

void B2DebugDraw_Init();

void B2DebugDraw_Draw();

b2DebugDraw* B2DebugDraw_GetDraw();

Vector2 B2Debug_ToScreen(b2Vec2 v);
Color B2Debug_HexToColor(b2HexColor hex, unsigned char alpha);


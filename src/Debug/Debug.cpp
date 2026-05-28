#include "Collision/Collisions.hpp"
#include "box2d/types.h"
#include "config.h"
#include "raylib.h"
#include "box2d/math_functions.h"
#include <math.h>


Vector2 B2Debug_ToScreen(b2Vec2 v)
{
    return { M_2_PX(v.x), M_2_PX(v.y) };
}

Color B2Debug_HexToColor(b2HexColor hex, unsigned char alpha)
{
    return {
        (unsigned char)((hex >> 16) & 0xFF),
        (unsigned char)((hex >>  8) & 0xFF),
        (unsigned char)( hex        & 0xFF),
        alpha
    };
}

static void CB_DrawPolygon(const b2Vec2* verts, int count,
                           b2HexColor color, void*)
{
    Color c = B2Debug_HexToColor(color, 255);
    for (int i = 0; i < count; ++i)
    {
        Vector2 a = B2Debug_ToScreen(verts[i]);
        Vector2 b = B2Debug_ToScreen(verts[(i + 1) % count]);
        DrawLineV(a, b, c);
    }
}

static void CB_DrawSolidPolygon(b2Transform xf, const b2Vec2* verts,
                                int count, float ,
                                b2HexColor color, void*)
{
    Color fill    = B2Debug_HexToColor(color,  64);
    Color outline = B2Debug_HexToColor(color, 255);

    Vector2 screen[16];
    for (int i = 0; i < count; ++i)
        screen[i] = B2Debug_ToScreen(b2TransformPoint(xf, verts[i]));

    for (int i = 1; i < count - 1; ++i)
        DrawTriangle(screen[0], screen[i], screen[i + 1], fill);

    for (int i = 0; i < count; ++i)
        DrawLineV(screen[i], screen[(i + 1) % count], outline);
}

static void CB_DrawCircle(b2Vec2 center, float radius,
                          b2HexColor color, void*)
{
    DrawCircleLinesV(B2Debug_ToScreen(center),
                     M_2_PX(radius),
                     B2Debug_HexToColor(color, 255));
}

static void CB_DrawSolidCircle(b2Transform xf, float radius,
                               b2HexColor color, void*)
{
    Vector2 c     = B2Debug_ToScreen(xf.p);
    float   r     = M_2_PX(radius);
    Color fill    = B2Debug_HexToColor(color,  64);
    Color outline = B2Debug_HexToColor(color, 255);

    DrawCircleV(c, r, fill);
    DrawCircleLinesV(c, r, outline);

    b2Vec2 tip = b2TransformPoint(xf, {radius, 0.0f});
    DrawLineV(c, B2Debug_ToScreen(tip), outline);
}

static void CB_DrawSolidCapsule(b2Vec2 p1, b2Vec2 p2, float radius,
                                b2HexColor color, void*)
{
    Color fill    = B2Debug_HexToColor(color,  64);
    Color outline = B2Debug_HexToColor(color, 255);
    float r       = M_2_PX(radius);

    Vector2 a = B2Debug_ToScreen(p1);
    Vector2 b = B2Debug_ToScreen(p2);

    DrawCircleV(a, r, fill);
    DrawCircleV(b, r, fill);
    DrawCircleLinesV(a, r, outline);
    DrawCircleLinesV(b, r, outline);

    float dx = b.x - a.x, dy = b.y - a.y;
    float len = sqrtf(dx*dx + dy*dy);
    if (len > 0.0001f)
    {
        float nx = -dy / len * r, ny = dx / len * r;
        Vector2 v0 = { a.x + nx, a.y + ny };
        Vector2 v1 = { a.x - nx, a.y - ny };
        Vector2 v2 = { b.x - nx, b.y - ny };
        Vector2 v3 = { b.x + nx, b.y + ny };
        DrawTriangle(v0, v2, v1, fill);
        DrawTriangle(v0, v3, v2, fill);
        DrawLineV(v0, v3, outline);
        DrawLineV(v1, v2, outline);
    }
}

static void CB_DrawLine(b2Vec2 p1, b2Vec2 p2,
                        b2HexColor color, void*)
{
    DrawLineV(B2Debug_ToScreen(p1),
              B2Debug_ToScreen(p2),
              B2Debug_HexToColor(color, 255));
}

static void CB_DrawTransform(b2Transform xf, void*)
{

    const float L = 0.3f;
    Vector2 o = B2Debug_ToScreen(xf.p);
    DrawLineV(o, B2Debug_ToScreen(b2TransformPoint(xf, { L, 0.f })), RED);
    DrawLineV(o, B2Debug_ToScreen(b2TransformPoint(xf, { 0.f, L })), RED);
}

static void CB_DrawPoint(b2Vec2 p, float size,
                         b2HexColor color, void*)
{
    
    float r = (size < 3.0f) ? 3.0f : size * 0.5f;
    DrawCircleV(B2Debug_ToScreen(p), r,
                B2Debug_HexToColor(color, 255));
}

static void CB_DrawString(b2Vec2 p, const char* s,
                          b2HexColor color, void*)
{
    Vector2 pos = B2Debug_ToScreen(p);
    DrawText(s, (int)pos.x, (int)pos.y, 12,
             B2Debug_HexToColor(color, 255));
}


static b2DebugDraw s_debugDraw = {};

void B2DebugDraw_Init()
{
    s_debugDraw = {};

    s_debugDraw.DrawPolygonFcn      = CB_DrawPolygon;
    s_debugDraw.DrawSolidPolygonFcn = CB_DrawSolidPolygon;
    s_debugDraw.DrawCircleFcn       = CB_DrawCircle;
    s_debugDraw.DrawSolidCircleFcn  = CB_DrawSolidCircle;
    s_debugDraw.DrawSolidCapsuleFcn = CB_DrawSolidCapsule;
    s_debugDraw.DrawLineFcn         = CB_DrawLine;
    s_debugDraw.DrawTransformFcn    = CB_DrawTransform;
    s_debugDraw.DrawPointFcn        = CB_DrawPoint;
    s_debugDraw.DrawStringFcn       = CB_DrawString;

    
    s_debugDraw.drawingBounds = {
        .lowerBound = { PX_2_M(-10000.f), PX_2_M(-10000.f) },
        .upperBound = { PX_2_M( 10000.f), PX_2_M( 10000.f) }
    };

    s_debugDraw.forceScale = 0.1f;
    s_debugDraw.jointScale = 1.0f;

    
    s_debugDraw.drawShapes        = true;
    s_debugDraw.drawJoints        = true;
    s_debugDraw.drawContacts      = false;
    s_debugDraw.drawContactNormals= false;
    s_debugDraw.drawBounds        = false;
    s_debugDraw.drawMass          = false;
    s_debugDraw.drawBodyNames     = false;

    s_debugDraw.context = nullptr;
}

void B2DebugDraw_Draw()
{
    b2World_Draw(gWorld, &s_debugDraw);
}

b2DebugDraw* B2DebugDraw_GetDraw()
{
    return &s_debugDraw;
}
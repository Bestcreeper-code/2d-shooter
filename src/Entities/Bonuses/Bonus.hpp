#pragma once

#include "Collision/Collisions.hpp"
#include "Object/Object.hpp"
#include "Sprites/Sprite.hpp"
#include "Textures/TextureCache.hpp"
#include "box2d/math_functions.h"
#include <string>


class Bonus : public PhysicsObject {
    protected:
        std::string spritePath;
        Sprite sprite;
    
    public:
        Bonus(Vector2 position, const std::string& path)
            : spritePath(path)
        {
            sprite.SetTexture(TextureCache::GetTexture(spritePath));
    
            float halfW = std::max(PX_2_M(sprite.texture.width  * sprite.scale) / 2.0f, 0.01f);
            float halfH = std::max(PX_2_M(sprite.texture.height * sprite.scale) / 2.0f, 0.01f);
    
            b2Filter filter;
            filter.categoryBits = COLLISION_LAYER_PLAYER_BONUS;
            filter.maskBits = COLLISION_LAYER_PLAYER;
    
            body = CreateBoxBody(
                gWorld,
                {PX_2_M(position.x), PX_2_M(position.y)},
                halfW, halfH,
                1.0f, 0.3f, 0.8f,
                false, filter
            );
        }

        void DrawSprite(b2Vec2 position) {
            sprite.Draw({
                M_2_PX(position.x) - (sprite.texture.width * sprite.scale) / 2.0f,
                M_2_PX(position.y) - (sprite.texture.height * sprite.scale) / 2.0f
            });
        }
};
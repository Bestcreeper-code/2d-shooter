#include "Bullets/BigBullet.hpp"
#include "Bullets/Bullet.hpp"
#include "Textures/TextureCache.hpp"
#include "config.hpp"



BigBullet::BigBullet(bool isPlayerBullet, Vector2 position, float dmg)
    : Bullet(isPlayerBullet, position, dmg)
{
    speed = 2.0f;

    sprite.SetTexture(TextureCache::GetTexture(IMG_DIR"Bullet.png"));
    sprite.scale = 3;

}





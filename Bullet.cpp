#include "Bullet.h"

Bullet::Bullet() = default;

Bullet::~Bullet() = default;

void Bullet::Draw() const
{
    setlinecolor(RGB(255, 155, 50));
    setfillcolor(RGB(200, 75, 10));
    fillcircle(position.x, position.y, RADIUS);//»æÖÆ×Óµ¯
}
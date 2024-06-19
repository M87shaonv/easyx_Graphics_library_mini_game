#ifndef ENEMY_H
#define ENEMY_H
#include <graphics.h>
#include "Animation.h"
#include "Config.h"
#include "Bullet.h"
#include "Player.h"
class Enemy
{
public:
    Enemy();
    ~Enemy();
    bool CheckBulletCollision(const Bullet& bullet);
    bool CheckPlayerCollision(const Player& player);
    void Move(const Player& player);
    void Draw(int delta);
    const POINT& GetPosition() const;//获取自身位置
private:
    const int MOVESPEED = 2;
    const int ENEMY_WIDTH = 80;//敌人宽度
    const int ENEMY_HEIGHT = 80;//敌人高度
    const int SHADOW_WIDTH = 48;//影子宽度
private:
    IMAGE shadow_img;//影子图片
    Animation* anim_enemy_left;
    Animation* anim_enemy_right;
    POINT position;
    FacingState facing = FacingState::Right;

    enum SpawnEdge //敌人出生边缘
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
};
#endif // ENEMY_H

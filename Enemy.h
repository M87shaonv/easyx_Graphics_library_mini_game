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
    const POINT& GetPosition() const;//��ȡ����λ��
private:
    const int MOVESPEED = 2;
    const int ENEMY_WIDTH = 80;//���˿��
    const int ENEMY_HEIGHT = 80;//���˸߶�
    const int SHADOW_WIDTH = 48;//Ӱ�ӿ��
private:
    IMAGE shadow_img;//Ӱ��ͼƬ
    Animation* anim_enemy_left;
    Animation* anim_enemy_right;
    POINT position;
    FacingState facing = FacingState::Right;

    enum SpawnEdge //���˳�����Ե
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
};
#endif // ENEMY_H

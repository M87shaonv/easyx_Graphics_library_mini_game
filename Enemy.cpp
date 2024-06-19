#include "Enemy.h"
#include <time.h>
Enemy::Enemy()
{
    loadimage(&shadow_img, _T("img/shadow_enemy.png"));
    anim_enemy_left = new Animation(_T("img/enemy_left_%d.png"), 6, 45);
    anim_enemy_right = new Animation(_T("img/enemy_right_%d.png"), 6, 45);

    srand(static_cast<unsigned int>(time(0)));//使用当前时间作为随机数生成器的种子
    SpawnEdge edge = (SpawnEdge)(rand() % 4);//随机生成边缘

    // 根据边缘位置生成坐标
    switch (edge)
    {
    case SpawnEdge::UP:
    case SpawnEdge::DOWN:
        position.x = rand() % SCREEN_WIDTH;
        position.y = (edge == SpawnEdge::UP) ? -SCREEN_HEIGHT : SCREEN_HEIGHT;
        break;
    case SpawnEdge::LEFT:
    case SpawnEdge::RIGHT:
        position.x = (edge == SpawnEdge::LEFT) ? -SCREEN_WIDTH : SCREEN_WIDTH;
        position.y = rand() % SCREEN_HEIGHT;
        break;
    }
}

Enemy::~Enemy()
{
    delete anim_enemy_left;
    delete anim_enemy_right;
}

bool Enemy::CheckBulletCollision(const Bullet& bullet)
{
    return false;
}

bool Enemy::CheckPlayerCollision(const Player& player)
{
    return false;
}

void Enemy::Move(const Player& player)
{
    const POINT& player_pos = player.GetPosition();
    int dx = player_pos.x - position.x;
    int dy = player_pos.y - position.y;
    double len_direction = sqrt(dx * dx + dy * dy);//计算两点之间的距离
    if (len_direction != 0) {
        double normalized_direction_x = dx / len_direction;//计算方向向量
        double normalized_direction_y = dy / len_direction;
        position.x += (int)(MOVESPEED * normalized_direction_x);//移动敌人
        position.y += (int)(MOVESPEED * normalized_direction_y);
    }
    facing = (player_pos.x < position.x) ? FacingState::Left : FacingState::Right;
}

void Enemy::Draw(int delta)
{
    int shadowpos_x = position.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
    int shadowpos_y = position.y + ENEMY_HEIGHT - 26; //计算高度的差值, 以便在敌人的底部绘制阴影图像
    putimage_alpha(shadowpos_x, shadowpos_y, &shadow_img);
    if (facing == FacingState::Left)//如果左移
        anim_enemy_left->Play(position.x, position.y, delta);
    else//如果右移
        anim_enemy_right->Play(position.x, position.y, delta);
}

const POINT& Enemy::GetPosition() const
{
    return position;
}
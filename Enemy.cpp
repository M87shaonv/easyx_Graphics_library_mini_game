#include "Enemy.h"

Enemy::Enemy()
{
    loadimage(&shadow_img, _T("img/shadow_enemy.png"));
    anim_enemy_left = new Animation(atlas_enemy_left, 45);
    anim_enemy_right = new Animation(atlas_enemy_right, 45);

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
    /*判断二维平面内点是否在矩形内,充要条件就是如果该点的水平坐标位于矩形左边界和右边界之间,
    同时垂直坐标位于矩形上边界和下边界之间,则该点在矩形内*/
    bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + ENEMY_WIDTH;
    bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + ENEMY_HEIGHT;
    return is_overlap_x && is_overlap_y;//如果在返回true
}

bool Enemy::CheckPlayerCollision(const Player& player)
{
    //将敌人中心位置作为点,判断点是否在玩家矩形内
    POINT check_position = { position.x + ENEMY_WIDTH / 2,position.y + ENEMY_HEIGHT / 2 };
    if (check_position.x >= player.CalculatePlayerLeft() && check_position.x <= player.CalculatePlayerRight()
        && check_position.y >= player.CalculatePlayerTop() && check_position.y <= player.CalculatePlayerBottom())
        return true;//如果在则返回true

    return false;
}

void Enemy::Move(const Player& player)
{
    const POINT& player_pos = player.GetPosition();
    int dx = player_pos.x - position.x;
    int dy = player_pos.y - position.y;
    double len_direction = sqrt(dx * dx + dy * dy);//计算两点之间的距离
    if (len_direction != 0)
    {
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
    int shadowpos_y = position.y + ENEMY_HEIGHT - 30; //计算高度的差值, 以便在敌人的底部绘制阴影图像
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

void Enemy::Hurt()
{
    alive = false;
}

bool Enemy::CheckAlive()
{
    return alive;
}
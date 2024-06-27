#include "Enemy.h"

Enemy::Enemy()
{
    loadimage(&shadow_img, _T("img/shadow_enemy.png"));
    anim_enemy_left = new Animation(atlas_enemy_left, 45);
    anim_enemy_right = new Animation(atlas_enemy_right, 45);

    SpawnEdge edge = (SpawnEdge)(rand() % 4);//������ɱ�Ե

    // ���ݱ�Եλ����������
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
    /*�ж϶�άƽ���ڵ��Ƿ��ھ�����,��Ҫ������������õ��ˮƽ����λ�ھ�����߽���ұ߽�֮��,
    ͬʱ��ֱ����λ�ھ����ϱ߽���±߽�֮��,��õ��ھ�����*/
    bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + ENEMY_WIDTH;
    bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + ENEMY_HEIGHT;
    return is_overlap_x && is_overlap_y;//����ڷ���true
}

bool Enemy::CheckPlayerCollision(const Player& player)
{
    //����������λ����Ϊ��,�жϵ��Ƿ�����Ҿ�����
    POINT check_position = { position.x + ENEMY_WIDTH / 2,position.y + ENEMY_HEIGHT / 2 };
    if (check_position.x >= player.CalculatePlayerLeft() && check_position.x <= player.CalculatePlayerRight()
        && check_position.y >= player.CalculatePlayerTop() && check_position.y <= player.CalculatePlayerBottom())
        return true;//������򷵻�true

    return false;
}

void Enemy::Move(const Player& player)
{
    const POINT& player_pos = player.GetPosition();
    int dx = player_pos.x - position.x;
    int dy = player_pos.y - position.y;
    double len_direction = sqrt(dx * dx + dy * dy);//��������֮��ľ���
    if (len_direction != 0)
    {
        double normalized_direction_x = dx / len_direction;//���㷽������
        double normalized_direction_y = dy / len_direction;
        position.x += (int)(MOVESPEED * normalized_direction_x);//�ƶ�����
        position.y += (int)(MOVESPEED * normalized_direction_y);
    }
    facing = (player_pos.x < position.x) ? FacingState::Left : FacingState::Right;
}

void Enemy::Draw(int delta)
{
    int shadowpos_x = position.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
    int shadowpos_y = position.y + ENEMY_HEIGHT - 30; //����߶ȵĲ�ֵ, �Ա��ڵ��˵ĵײ�������Ӱͼ��
    putimage_alpha(shadowpos_x, shadowpos_y, &shadow_img);
    if (facing == FacingState::Left)//�������
        anim_enemy_left->Play(position.x, position.y, delta);
    else//�������
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
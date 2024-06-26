#ifndef _AUTOENEMY_H_
#define _AUTOENEMY_H_
#include "Enemy.h"
#include <iostream>
#include "Player.h"
#include <chrono>
#include <thread>
#include <vector>
extern Player player;//��Ҷ���

class AutoEnemy
{
public:
    AutoEnemy()
    {
        loadimage(&shadow_img, _T("img/shadow_enemy.png"));
        anim_autoenemy_idle = new Animation(atlas_autoenemy_idle, 45);
        anim_autoenemy_blast = new Animation(atlas_autoenemy_blast, 45);
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
    ~AutoEnemy()
    {
        delete anim_autoenemy_idle;
        delete anim_autoenemy_blast;
    }
    bool CheckBulletCollision(const Bullet& bullet)
    {
             /*�ж϶�άƽ���ڵ��Ƿ��ھ�����,��Ҫ������������õ��ˮƽ����λ�ھ�����߽���ұ߽�֮��,
    ͬʱ��ֱ����λ�ھ����ϱ߽���±߽�֮��,��õ��ھ�����*/
        bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + ENEMY_WIDTH;
        bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + ENEMY_HEIGHT;
        return is_overlap_x && is_overlap_y;//����ڷ���true
    }
    bool CheckPlayerCollision(const Player& player)
    {
            //����������λ����Ϊ��,�жϵ��Ƿ�����Ҿ�����
        POINT check_position = { position.x + ENEMY_WIDTH / 2,position.y + ENEMY_HEIGHT / 2 };
        if (check_position.x >= player.CalculatePlayerLeft() && check_position.x <= player.CalculatePlayerRight()
            && check_position.y >= player.CalculatePlayerTop() && check_position.y <= player.CalculatePlayerBottom())
            return true;//������򷵻�true

        return false;
    }
    void Move(const Player& player)
    {
        ++blastCounter;
        if (blastCounter >= blastTime && !is_blasting)
        {
            Blast();
            is_blasting = true;
        }
        const POINT& player_pos = player.GetPosition();
        int dx = player_pos.x - position.x;
        int dy = player_pos.y - position.y;
        double len_direction = sqrt(dx * dx + dy * dy); // ��������֮��ľ���

        const double stop_distance = 400; // ֹͣ�ƶ��ľ���
        const double retreat_distance = 350; // ����ƶ��ľ���
        if (is_move_player) //Ҫ��ըʱ������ƶ�
        {
            double normalized_direction_x = dx / len_direction; // ���㷽������
            double normalized_direction_y = dy / len_direction;
            position.x += (int)((MOVESPEED - 1) * normalized_direction_x); // �ƶ�����
            position.y += (int)((MOVESPEED - 1) * normalized_direction_y);
            return;
        }
        if (len_direction > stop_distance)
        {
            // ����������ֹͣ���룬������ƶ�
            if (len_direction != 0)
            {
                double normalized_direction_x = dx / len_direction; // ���㷽������
                double normalized_direction_y = dy / len_direction;
                position.x += (int)(MOVESPEED * normalized_direction_x); // �ƶ�����
                position.y += (int)(MOVESPEED * normalized_direction_y);
            }
        }
        else if (len_direction < retreat_distance)
        {
           // �������С�ڳ��˾��룬����ƶ�
            if (len_direction != 0)
            {
                double normalized_direction_x = dx / len_direction; // ���㷽������
                double normalized_direction_y = dy / len_direction;

                // ��¼��ǰλ��
                POINT current_position = position;

                // ����ƶ�����
                if (back_move)
                {
                    position.x -= (int)(MOVESPEED * normalized_direction_x);
                    position.y -= (int)(MOVESPEED * normalized_direction_y);
                }

                // ȷ�����˲�������Ļ��Ե
                if (position.x < 0) position.x = 0;
                if (position.x > SCREEN_WIDTH - ENEMY_WIDTH) position.x = SCREEN_WIDTH - ENEMY_WIDTH;
                if (position.y < 0) position.y = 0;
                if (position.y > SCREEN_HEIGHT - ENEMY_HEIGHT) position.y = SCREEN_HEIGHT - ENEMY_HEIGHT;

                // �����ǰλ������Ļ��Ե���������������ƶ�
                bool atLeftEdge = position.x <= 0;
                bool atRightEdge = position.x >= SCREEN_WIDTH - ENEMY_WIDTH;
                bool atTopEdge = position.y <= 0;
                bool atBottomEdge = position.y >= SCREEN_HEIGHT - ENEMY_HEIGHT;

                if (atLeftEdge && atTopEdge) // ���Ͻ�
                {
                    if (dx > 0 && dy > 0) // ��������½Ƿ���
                    {
                        position.x += MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx > 0) // ������Ҳ�
                    {
                        position.x += MOVESPEED;
                    }
                    else if (dy > 0) // ������²�
                    {
                        position.y += MOVESPEED;
                    }
                }
                else if (atLeftEdge && atBottomEdge) // ���½�
                {
                    if (dx > 0 && dy < 0) // ��������ϽǷ���
                    {
                        position.x += MOVESPEED;
                        position.y -= MOVESPEED;
                    }
                    else if (dx > 0 && dy > 0) // ��������½Ƿ���
                    {
                        position.x += MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx > 0) // ������Ҳ�
                    {
                        position.x += MOVESPEED;
                    }
                    else if (dy < 0) // ������ϲ�
                    {
                        position.y -= MOVESPEED;
                    }
                }
                else if (atRightEdge && atTopEdge) // ���Ͻ�
                {
                    if (dx < 0 && dy > 0) // ��������½Ƿ���
                    {
                        position.x -= MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx < 0) // ��������
                    {
                        position.x -= MOVESPEED;
                    }
                    else if (dy > 0) // ������²�
                    {
                        position.y += MOVESPEED;
                    }
                }
                else if (atRightEdge && atBottomEdge) // ���½�
                {
                    if (dx < 0 && dy < 0) // ��������ϽǷ���
                    {
                        position.x -= MOVESPEED;
                        position.y -= MOVESPEED;
                    }
                    else if (dx < 0 && dy > 0) // ��������½Ƿ���
                    {
                        position.x -= MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx < 0) // ��������
                    {
                        position.x -= MOVESPEED;
                    }
                    else if (dy < 0) // ������ϲ�
                    {
                        position.y -= MOVESPEED;
                    }
                }
                else if (atLeftEdge) // ����Ե
                {
                    if (dx < 0)
                    {
                        position.y += (dy < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atRightEdge) // �Ҳ��Ե
                {
                    if (dx > 0)
                    {
                        position.y += (dy < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atTopEdge) // �ϲ��Ե
                {
                    if (dy < 0)
                    {
                        position.x += (dx < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atBottomEdge) // �²��Ե
                {
                    if (dy > 0)
                    {
                        position.x += (dx < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }

                back_move = !back_move; // �ı��ƶ�����
            }
            else
            {
                back_move = true; // �ص�ԭ�����ƶ�����
            }
        }

        // ������һ���λ��
        last_position = position;
    }

    void Draw(int delta)
    {          // ���ű�ը����
        if (is_blasting)
        {
            anim_autoenemy_blast->Play(position.x, position.y, 45);
        }
        int shadowpos_x = position.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
        int shadowpos_y = position.y + ENEMY_HEIGHT - 30; //����߶ȵĲ�ֵ, �Ա��ڵ��˵ĵײ�������Ӱͼ��
        putimage_alpha(shadowpos_x, shadowpos_y, &shadow_img);
        if (is_blasting) return;
        anim_autoenemy_idle->Play(position.x, position.y, delta);
    }
    const POINT& GetPosition() const
    {
        return position;
    }
    void Hurt()
    {
        alive = false;
    }
    bool CheckAlive()
    {
        return alive;
    }
    void Blast()
    {
        mciSendString(_T("play blast from 0"), NULL, 0, NULL);//���ű�ը��Ч
        is_move_player = true; // ������ƶ�

// ����һ���߳��������ӳ�ɾ��
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            CheckAndDamagePlayer(); // ��鲢�˺����

            blasted = true; // ���ñ�ը״̬
            }).detach();
    }

    void CheckAndDamagePlayer()
    {
        const POINT& player_pos = player.GetPosition();
        int dx = player_pos.x - position.x;
        int dy = player_pos.y - position.y;
        double distance = sqrt(dx * dx + dy * dy);

        const int blastRadius = 80; // ��ը��Χ�뾶
        if (distance <= blastRadius)
        {
            --player.player_health;
        }
    }
    bool CheckBlast()
    {
        return blasted;
    }

private:
    const int MOVESPEED = 4;
    const int ENEMY_WIDTH = 80;//���˿��
    const int ENEMY_HEIGHT = 80;//���˸߶�
    const int SHADOW_WIDTH = 48;//Ӱ�ӿ��
private:
    IMAGE shadow_img;//Ӱ��ͼƬ
    bool is_move_player = false; // �Ƿ�������ƶ�
    bool is_shooting = false;
    Animation* anim_autoenemy_idle;
    Animation* anim_autoenemy_blast;
    bool back_move = true; // �Ƿ�����ƶ�
    POINT last_position = position; // ��¼��һ���λ��

    POINT position;
    bool alive = true;
    bool blasted = false;
    bool is_blasting = false;
    int blastCounter = 0;
    int blastTime = 1000; // ��ըʱ��

    enum SpawnEdge //���˳�����Ե
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
};
#endif

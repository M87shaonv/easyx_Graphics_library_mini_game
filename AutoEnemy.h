#ifndef _AUTOENEMY_H_
#define _AUTOENEMY_H_
#include "Enemy.h"
#include <iostream>
#include "Player.h"
#include <chrono>
#include <thread>
#include <vector>
extern Player player;//玩家对象

class AutoEnemy
{
public:
    AutoEnemy()
    {
        loadimage(&shadow_img, _T("img/shadow_enemy.png"));
        anim_autoenemy_idle = new Animation(atlas_autoenemy_idle, 45);
        anim_autoenemy_blast = new Animation(atlas_autoenemy_blast, 45);
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
    ~AutoEnemy()
    {
        delete anim_autoenemy_idle;
        delete anim_autoenemy_blast;
    }
    bool CheckBulletCollision(const Bullet& bullet)
    {
             /*判断二维平面内点是否在矩形内,充要条件就是如果该点的水平坐标位于矩形左边界和右边界之间,
    同时垂直坐标位于矩形上边界和下边界之间,则该点在矩形内*/
        bool is_overlap_x = bullet.position.x >= position.x && bullet.position.x <= position.x + ENEMY_WIDTH;
        bool is_overlap_y = bullet.position.y >= position.y && bullet.position.y <= position.y + ENEMY_HEIGHT;
        return is_overlap_x && is_overlap_y;//如果在返回true
    }
    bool CheckPlayerCollision(const Player& player)
    {
            //将敌人中心位置作为点,判断点是否在玩家矩形内
        POINT check_position = { position.x + ENEMY_WIDTH / 2,position.y + ENEMY_HEIGHT / 2 };
        if (check_position.x >= player.CalculatePlayerLeft() && check_position.x <= player.CalculatePlayerRight()
            && check_position.y >= player.CalculatePlayerTop() && check_position.y <= player.CalculatePlayerBottom())
            return true;//如果在则返回true

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
        double len_direction = sqrt(dx * dx + dy * dy); // 计算两点之间的距离

        const double stop_distance = 400; // 停止移动的距离
        const double retreat_distance = 350; // 向后移动的距离
        if (is_move_player) //要爆炸时向玩家移动
        {
            double normalized_direction_x = dx / len_direction; // 计算方向向量
            double normalized_direction_y = dy / len_direction;
            position.x += (int)((MOVESPEED - 1) * normalized_direction_x); // 移动敌人
            position.y += (int)((MOVESPEED - 1) * normalized_direction_y);
            return;
        }
        if (len_direction > stop_distance)
        {
            // 如果距离大于停止距离，向玩家移动
            if (len_direction != 0)
            {
                double normalized_direction_x = dx / len_direction; // 计算方向向量
                double normalized_direction_y = dy / len_direction;
                position.x += (int)(MOVESPEED * normalized_direction_x); // 移动敌人
                position.y += (int)(MOVESPEED * normalized_direction_y);
            }
        }
        else if (len_direction < retreat_distance)
        {
           // 如果距离小于撤退距离，向后移动
            if (len_direction != 0)
            {
                double normalized_direction_x = dx / len_direction; // 计算方向向量
                double normalized_direction_y = dy / len_direction;

                // 记录当前位置
                POINT current_position = position;

                // 向后移动敌人
                if (back_move)
                {
                    position.x -= (int)(MOVESPEED * normalized_direction_x);
                    position.y -= (int)(MOVESPEED * normalized_direction_y);
                }

                // 确保敌人不超出屏幕边缘
                if (position.x < 0) position.x = 0;
                if (position.x > SCREEN_WIDTH - ENEMY_WIDTH) position.x = SCREEN_WIDTH - ENEMY_WIDTH;
                if (position.y < 0) position.y = 0;
                if (position.y > SCREEN_HEIGHT - ENEMY_HEIGHT) position.y = SCREEN_HEIGHT - ENEMY_HEIGHT;

                // 如果当前位置在屏幕边缘，则向其他方向移动
                bool atLeftEdge = position.x <= 0;
                bool atRightEdge = position.x >= SCREEN_WIDTH - ENEMY_WIDTH;
                bool atTopEdge = position.y <= 0;
                bool atBottomEdge = position.y >= SCREEN_HEIGHT - ENEMY_HEIGHT;

                if (atLeftEdge && atTopEdge) // 左上角
                {
                    if (dx > 0 && dy > 0) // 玩家在右下角方向
                    {
                        position.x += MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx > 0) // 玩家在右侧
                    {
                        position.x += MOVESPEED;
                    }
                    else if (dy > 0) // 玩家在下侧
                    {
                        position.y += MOVESPEED;
                    }
                }
                else if (atLeftEdge && atBottomEdge) // 左下角
                {
                    if (dx > 0 && dy < 0) // 玩家在右上角方向
                    {
                        position.x += MOVESPEED;
                        position.y -= MOVESPEED;
                    }
                    else if (dx > 0 && dy > 0) // 玩家在右下角方向
                    {
                        position.x += MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx > 0) // 玩家在右侧
                    {
                        position.x += MOVESPEED;
                    }
                    else if (dy < 0) // 玩家在上侧
                    {
                        position.y -= MOVESPEED;
                    }
                }
                else if (atRightEdge && atTopEdge) // 右上角
                {
                    if (dx < 0 && dy > 0) // 玩家在左下角方向
                    {
                        position.x -= MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx < 0) // 玩家在左侧
                    {
                        position.x -= MOVESPEED;
                    }
                    else if (dy > 0) // 玩家在下侧
                    {
                        position.y += MOVESPEED;
                    }
                }
                else if (atRightEdge && atBottomEdge) // 右下角
                {
                    if (dx < 0 && dy < 0) // 玩家在左上角方向
                    {
                        position.x -= MOVESPEED;
                        position.y -= MOVESPEED;
                    }
                    else if (dx < 0 && dy > 0) // 玩家在左下角方向
                    {
                        position.x -= MOVESPEED;
                        position.y += MOVESPEED;
                    }
                    else if (dx < 0) // 玩家在左侧
                    {
                        position.x -= MOVESPEED;
                    }
                    else if (dy < 0) // 玩家在上侧
                    {
                        position.y -= MOVESPEED;
                    }
                }
                else if (atLeftEdge) // 左侧边缘
                {
                    if (dx < 0)
                    {
                        position.y += (dy < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atRightEdge) // 右侧边缘
                {
                    if (dx > 0)
                    {
                        position.y += (dy < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atTopEdge) // 上侧边缘
                {
                    if (dy < 0)
                    {
                        position.x += (dx < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }
                else if (atBottomEdge) // 下侧边缘
                {
                    if (dy > 0)
                    {
                        position.x += (dx < 0) ? MOVESPEED : -MOVESPEED;
                    }
                }

                back_move = !back_move; // 改变移动方向
            }
            else
            {
                back_move = true; // 回到原来的移动方向
            }
        }

        // 更新上一秒的位置
        last_position = position;
    }

    void Draw(int delta)
    {          // 播放爆炸动画
        if (is_blasting)
        {
            anim_autoenemy_blast->Play(position.x, position.y, 45);
        }
        int shadowpos_x = position.x + (ENEMY_WIDTH / 2 - SHADOW_WIDTH / 2);
        int shadowpos_y = position.y + ENEMY_HEIGHT - 30; //计算高度的差值, 以便在敌人的底部绘制阴影图像
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
        mciSendString(_T("play blast from 0"), NULL, 0, NULL);//播放爆炸音效
        is_move_player = true; // 向玩家移动

// 创建一个线程来处理延迟删除
        std::thread([this]() {
            std::this_thread::sleep_for(std::chrono::seconds(3));
            CheckAndDamagePlayer(); // 检查并伤害玩家

            blasted = true; // 设置爆炸状态
            }).detach();
    }

    void CheckAndDamagePlayer()
    {
        const POINT& player_pos = player.GetPosition();
        int dx = player_pos.x - position.x;
        int dy = player_pos.y - position.y;
        double distance = sqrt(dx * dx + dy * dy);

        const int blastRadius = 80; // 爆炸范围半径
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
    const int ENEMY_WIDTH = 80;//敌人宽度
    const int ENEMY_HEIGHT = 80;//敌人高度
    const int SHADOW_WIDTH = 48;//影子宽度
private:
    IMAGE shadow_img;//影子图片
    bool is_move_player = false; // 是否向玩家移动
    bool is_shooting = false;
    Animation* anim_autoenemy_idle;
    Animation* anim_autoenemy_blast;
    bool back_move = true; // 是否向后移动
    POINT last_position = position; // 记录上一秒的位置

    POINT position;
    bool alive = true;
    bool blasted = false;
    bool is_blasting = false;
    int blastCounter = 0;
    int blastTime = 1000; // 爆炸时间

    enum SpawnEdge //敌人出生边缘
    {
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
};
#endif

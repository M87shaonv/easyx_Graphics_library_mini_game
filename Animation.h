#ifndef ANIMATION_H
#define ANIMATION_H
#include <graphics.h>
#include <vector>
#include "Atlas.h"

//定义FacingState枚举类型,用于表示角色的朝向状态
enum FacingState
{
    Left,
    Right,
    Move,
};

class Animation
{
public:
    //atlas表示动画素材, interval表示每帧持续时间, 单位为毫秒
    Animation(Atlas* atlas, int interval)
    {
        anim_atlas = atlas;
        interval_ms = interval;
    }
    ~Animation() = default;

    void Play(int x, int y, int delta)//delta表示距离上次调用play过去多久
    {
        timer += delta;//累加计时器
        if (timer >= interval_ms)//如果计时器超过了帧间隔,则切换到下一帧
        {
            //更新帧索引
            frame_index = (++frame_index) % anim_atlas->frame_sequence.size();
            timer = 0;
        }
        putimage_alpha(x, y, anim_atlas->frame_sequence[frame_index]);//显示当前帧
    }

private:
    int timer = 0;//动画计时器
    int frame_index = 0;//动画帧索引
    int interval_ms = 0;//帧间隔
private:
    Atlas* anim_atlas;//动画素材
};
#endif // !ANIMATION
#ifndef ANIMATION_H
#define ANIMATION_H
#include <graphics.h>
#include <vector>
#include "Atlas.h"

//����FacingStateö������,���ڱ�ʾ��ɫ�ĳ���״̬
enum FacingState
{
    Left,
    Right,
    Move,
};

class Animation
{
public:
    //atlas��ʾ�����ز�, interval��ʾÿ֡����ʱ��, ��λΪ����
    Animation(Atlas* atlas, int interval)
    {
        anim_atlas = atlas;
        interval_ms = interval;
    }
    ~Animation() = default;

    void Play(int x, int y, int delta)//delta��ʾ�����ϴε���play��ȥ���
    {
        timer += delta;//�ۼӼ�ʱ��
        if (timer >= interval_ms)//�����ʱ��������֡���,���л�����һ֡
        {
            //����֡����
            frame_index = (++frame_index) % anim_atlas->frame_sequence.size();
            timer = 0;
        }
        putimage_alpha(x, y, anim_atlas->frame_sequence[frame_index]);//��ʾ��ǰ֡
    }

private:
    int timer = 0;//������ʱ��
    int frame_index = 0;//����֡����
    int interval_ms = 0;//֡���
private:
    Atlas* anim_atlas;//�����ز�
};
#endif // !ANIMATION
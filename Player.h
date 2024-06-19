#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"

class Player
{
public:
    Player();
    ~Player();
    void ProcessEvent(const ExMessage& msg);//������Ҳ�����Ϣ
    void Move();//��������ƶ�
    void Draw(int delta);//�������
    const POINT& GetPosition() const;//��ȡ���λ��
private:
    const INT PLAYERSPEED = 5;//��ҳ�ʼ��ʼ�ٶ�
    const INT PLAYER_WIDTH = 80;//��ҿ�ȳ���
    const INT PLAYER_HEIGHT = 80;//��Ҹ߶ȳ���
    const INT SHADOW_WIDTH = 30;//Ӱ�ӿ�ȳ���
private:
    IMAGE shadowImg;//Ӱ��ͼƬ
    Animation* anim_player_left;//����������ƶ���
    Animation* anim_player_right;
    FacingState facing = FacingState::Right;//��ʼ���泯����
    POINT playerPos = { 500,500 };//��ʼ��λ��
    bool isMoveUp = false;
    bool isMoveDown = false;
    bool isMoveLeft = false;
    bool isMoveRight = false;
    int moveSpeedX = 0;//���X���ٶ�
    int moveSpeedY = 0;//���Y���ٶ�
};
#endif
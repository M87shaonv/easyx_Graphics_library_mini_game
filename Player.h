#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"
#include "Atlas.h"
class Player
{
public:
    Player();
    ~Player();
    void ProcessEvent(const ExMessage& msg);//������Ҳ�����Ϣ
    void Move();//��������ƶ�
    void Draw(int delta);//�������
    const POINT& GetPosition() const;//��ȡ���λ��
    const int CalculatePlayerLeft() const;//���������߽�
    const int CalculatePlayerRight() const;//��������ұ߽�
    const int CalculatePlayerTop() const;//��������ϱ߽�
    const int CalculatePlayerBottom() const;//��������±߽�
public:
    const INT PLAYERSPEED = 3;//��ҳ�ʼ��ʼ�ٶ�
    const INT PLAYER_WIDTH = 82;//��ҿ�ȳ���
    const INT PLAYER_HEIGHT = 82;//��Ҹ߶ȳ���
    const INT SHADOW_WIDTH = 32;//Ӱ�ӿ�ȳ���
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
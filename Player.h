#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"
#include "Atlas.h"
class Player
{
public:
    Player();
    ~Player();
    void ResetMove();//��������ƶ�״̬,������ʱ����,�����Ͳ�����Ϊ�����ƶ��ٶȶ����µĴ���,����뿴������������ȥ�����������������
    void ProcessEvent(const ExMessage& msg);//������Ҳ�����Ϣ
    void Move();//��������ƶ�
    void Draw(int delta);//�������
    const POINT& GetPosition() const;//��ȡ���λ��
    const int CalculatePlayerLeft() const;//���������߽�
    const int CalculatePlayerRight() const;//��������ұ߽�
    const int CalculatePlayerTop() const;//��������ϱ߽�
    const int CalculatePlayerBottom() const;//��������±߽�
    void reset();//�������״̬

public:
    const INT PLAYERSPEED = 3;//��ҳ�ʼ��ʼ�ٶ�
    const INT PLAYER_WIDTH = 80;//��ҿ�ȳ���
    const INT PLAYER_HEIGHT = 80;//��Ҹ߶ȳ���
    const INT SHADOW_WIDTH = 32;//Ӱ�ӿ�ȳ���
    const INT PALYERHEALTH = 1;//��ҳ�ʼ����ֵ

public:
    int playerspeed = PLAYERSPEED;//����ٶ�
    int player_width = PLAYER_WIDTH;//��ҿ��
    int player_height = PLAYER_HEIGHT;//��Ҹ߶�
    int shadow_width = SHADOW_WIDTH;//Ӱ�ӿ��
    int player_bullets = 3;//��ҳ�ʼ�ӵ���
    int player_health = PALYERHEALTH;//�������ֵ
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
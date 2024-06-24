#include "Player.h"
#include "Config.h"

Player::Player()
{
    loadimage(&shadowImg, _T("img/shadow_player.png"));//����Ӱ��ͼƬ
    anim_player_left = new Animation(atlas_player_left, 160);//1���ڲ���6��ͼƬ
    anim_player_right = new Animation(atlas_player_right, 160);
}

Player::~Player()
{
    delete anim_player_left;
    delete anim_player_right;
}

void Player::ProcessEvent(const ExMessage& msg)
{
    if (msg.message == WM_KEYDOWN)//���msg�Ƿ���һ�����̰������µ���Ϣ,WM_KEYDOWN ��һ��Windows��Ϣ��������ʾ�����ϵ�һ����������
    {
        //���ݰ������������ִ�в�ͬ����,������΢��ٷ��ĵ��鿴�������ֵ
        switch (msg.vkcode)
        {
        case VK_UP:
            isMoveUp = true;
            break;
        case VK_DOWN:
            isMoveDown = true;
            break;
        case VK_LEFT:
            isMoveLeft = true;
            facing = FacingState::Left;
            break;
        case VK_RIGHT:
            isMoveRight = true;
            facing = FacingState::Right;
            break;
        }
    }
    else if (msg.message == WM_KEYUP)//����̧��
    {
        switch (msg.vkcode)
        {
        case VK_UP:
            isMoveUp = false;
            break;
        case VK_DOWN:
            isMoveDown = false;
            break;
        case VK_LEFT:
            isMoveLeft = false;
            break;
        case VK_RIGHT:
            isMoveRight = false;
            break;
        default:
            facing = FacingState::Move;
            break;
        }
    }
}

void Player::Move()
{
    //�����������Ϸ�������˶�
    if (playerPos.x < 0) playerPos.x = 0;
    else if (playerPos.x + PLAYER_WIDTH > SCREEN_WIDTH) playerPos.x = SCREEN_WIDTH - PLAYER_WIDTH;
    if (playerPos.y < 0) playerPos.y = 0;
    else if (playerPos.y + PLAYER_HEIGHT > SCREEN_HEIGHT) playerPos.y = SCREEN_HEIGHT - PLAYER_HEIGHT;

    moveSpeedX = 0;//���X���ٶ�
    moveSpeedY = 0;//���Y���ٶ�
    if (isMoveUp) {
        moveSpeedY -= PLAYERSPEED;
    }
    if (isMoveDown) {
        moveSpeedY += PLAYERSPEED;
    }
    if (isMoveLeft) {
        moveSpeedX -= PLAYERSPEED;
        facing = FacingState::Left;
    }
    if (isMoveRight) {
        moveSpeedX += PLAYERSPEED;
        facing = FacingState::Right;
    }

    /*�� 2 ����ƽ�������㣬�õ�Լ 1.414�����ֵ��б���ƶ�ʱ X �� Y ���ٶȵ��ӵĽ����
    ʹ�� static_cast<int> ��������ת��Ϊ�������ͣ���ȷ���ٶ�������ֵ��
    ��������Ĺ���ȷ����б���ƶ�ʱ�����ٶ��� X ��� Y ���ϱ���һ��*/
    if (moveSpeedX != 0 && moveSpeedY != 0) {
        moveSpeedX = static_cast<int>(moveSpeedX / sqrt(2));
        moveSpeedY = static_cast<int>(moveSpeedY / sqrt(2));
    }
    playerPos.x += moveSpeedX;
    playerPos.y += moveSpeedY;
}
//delta��ʾ�����ϴε���play��ȥ���
void Player::Draw(int delta)
{        //��������ҽ�ɫ��ȵ�һ�����Ӱͼ���ȵ�һ��֮��Ĳ�ֵ,�Ա�����ҽ�ɫ�����м������Ӱͼ��
    int shadowpos_x = playerPos.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
    //��������ҽ�ɫ�߶ȵĲ�ֵ,�Ա�����ҽ�ɫ�ĵײ�������Ӱͼ��
    int shadowpos_y = playerPos.y + PLAYER_HEIGHT - 10;
    putimage_alpha(shadowpos_x, shadowpos_y, &shadowImg);
    if (facing == FacingState::Left)//�������
        anim_player_left->Play(playerPos.x, playerPos.y, delta);
    else//�������
        anim_player_right->Play(playerPos.x, playerPos.y, delta);
}
const POINT& Player::GetPosition() const
{
    return playerPos;
}
// ���㲢������Ҿ��ε���߽�
const int Player::CalculatePlayerLeft() const
{
    return playerPos.x - PLAYER_WIDTH / 2;
}

// ���㲢������Ҿ��ε��ұ߽�
const int Player::CalculatePlayerRight() const
{
    return playerPos.x + PLAYER_WIDTH / 2;
}

// ���㲢������Ҿ��εĶ��߽�
const int Player::CalculatePlayerTop() const
{
    return playerPos.y - PLAYER_HEIGHT / 2;
}

// ���㲢������Ҿ��εĵױ߽�
const int Player::CalculatePlayerBottom() const
{
    return playerPos.y + PLAYER_HEIGHT / 2;
}
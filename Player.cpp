#include "Player.h"
#include "Config.h"
#include "Bullet.h"

Player::Player()
{
    loadimage(&shadowImg, _T("img/shadow_player.png")); //加载影子图片
    anim_player_left = new Animation(atlas_player_left, 160);//1秒内播放6张图片
    anim_player_right = new Animation(atlas_player_right, 160);
}

Player::~Player()
{
    delete anim_player_left;
    delete anim_player_right;
}
void Player::ResetMove()
{
    isMoveUp = false;
    isMoveDown = false;
    isMoveLeft = false;
    isMoveRight = false;
}
void Player::ProcessEvent(const ExMessage& msg)
{
    if (msg.message == WM_KEYDOWN)//检查msg是否是一个键盘按键按下的消息,WM_KEYDOWN 是一个Windows消息常量，表示键盘上的一个键被按下
    {
        //根据按键的虚拟键码执行不同操作,可以在微软官方文档查看虚拟键码值
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
    else if (msg.message == WM_KEYUP)//按键抬起
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
    moveSpeedX = 0;//玩家X轴速度
    moveSpeedY = 0;//玩家Y轴速度
    if (isMoveUp)
    {
        moveSpeedY -= playerspeed;
    }
    if (isMoveDown)
    {
        moveSpeedY += playerspeed;
    }
    if (isMoveLeft)
    {
        moveSpeedX -= playerspeed;
        facing = FacingState::Left;
    }
    if (isMoveRight)
    {
        moveSpeedX += playerspeed;
        facing = FacingState::Right;
    }

    /*对 2 进行平方根运算，得到约 1.414。这个值是斜着移动时 X 和 Y 轴速度叠加的结果。
    使用 static_cast<int> 将计算结果转换为整数类型，以确保速度是整数值。
    这个调整的过程确保了斜着移动时的总速度在 X 轴和 Y 轴上保持一致*/
    if (moveSpeedX != 0 && moveSpeedY != 0)
    {
        moveSpeedX = static_cast<int>(moveSpeedX / sqrt(2));
        moveSpeedY = static_cast<int>(moveSpeedY / sqrt(2));
    }
    playerPos.x += moveSpeedX;
    playerPos.y += moveSpeedY;

    //限制玩家在游戏窗口内运动
    if (playerPos.x < 0) playerPos.x = 0;
    else if (playerPos.x + player_width > SCREEN_WIDTH) playerPos.x = SCREEN_WIDTH - player_width;
    if (playerPos.y < 0) playerPos.y = 0;
    else if (playerPos.y + player_width > SCREEN_HEIGHT) playerPos.y = SCREEN_HEIGHT - player_width;
}

//delta表示距离上次调用play过去多久
void Player::Draw(int delta)
{        //计算了玩家角色宽度的一半和阴影图像宽度的一半之间的差值,以便在玩家角色的正中间绘制阴影图像
    int shadowpos_x = playerPos.x + (player_width / 2 - shadow_width / 2);
    //计算了玩家角色高度的差值,以便在玩家角色的底部绘制阴影图像
    int shadowpos_y = playerPos.y + player_height - 10;
    putimage_alpha(shadowpos_x, shadowpos_y, &shadowImg);
    if (facing == FacingState::Left)//如果左移
        anim_player_left->Play(playerPos.x, playerPos.y, delta);
    else//如果右移
        anim_player_right->Play(playerPos.x, playerPos.y, delta);
}
const POINT& Player::GetPosition() const
{
    return playerPos;
}
// 计算并返回玩家矩形的左边界
const int Player::CalculatePlayerLeft() const
{
    return playerPos.x - player_width / 2;
}

// 计算并返回玩家矩形的右边界
const int Player::CalculatePlayerRight() const
{
    return playerPos.x + player_width / 2;
}

// 计算并返回玩家矩形的顶边界
const int Player::CalculatePlayerTop() const
{
    return playerPos.y - player_height / 2;
}

// 计算并返回玩家矩形的底边界
const int Player::CalculatePlayerBottom() const
{
    return playerPos.y + player_height / 2;
}

void Player::reset()
{
    playerPos = { 500,500 };
    facing = FacingState::Right;
    isMoveUp = false;
    isMoveDown = false;
    isMoveLeft = false;
    isMoveRight = false;
    moveSpeedX = 0;//玩家X轴速度
    moveSpeedY = 0;//玩家Y轴速度
    player_bullets = 3;
    player_health = PALYERHEALTH;//玩家血量
    playerspeed = PLAYERSPEED;//玩家速度
    player_width = PLAYER_WIDTH;//玩家宽度
    player_height = PLAYER_HEIGHT;//玩家高度
    shadow_width = SHADOW_WIDTH;//影子宽度
}
#ifndef PLAYER_H
#define PLAYER_H
#include "Animation.h"
#include "Atlas.h"
class Player
{
public:
    Player();
    ~Player();
    void ResetMove();//重置玩家移动状态,在升级时调用,这样就不会因为增加移动速度而导致的错乱,如果想看可以在主函数去掉这个函数调用试试
    void ProcessEvent(const ExMessage& msg);//处理玩家操作信息
    void Move();//处理玩家移动
    void Draw(int delta);//绘制玩家
    const POINT& GetPosition() const;//获取玩家位置
    const int CalculatePlayerLeft() const;//计算玩家左边界
    const int CalculatePlayerRight() const;//计算玩家右边界
    const int CalculatePlayerTop() const;//计算玩家上边界
    const int CalculatePlayerBottom() const;//计算玩家下边界
    void reset();//重置玩家状态

public:
    const INT PLAYERSPEED = 3;//玩家初始初始速度
    const INT PLAYER_WIDTH = 80;//玩家宽度常量
    const INT PLAYER_HEIGHT = 80;//玩家高度常量
    const INT SHADOW_WIDTH = 32;//影子宽度常量
    const INT PALYERHEALTH = 1;//玩家初始生命值

public:
    int playerspeed = PLAYERSPEED;//玩家速度
    int player_width = PLAYER_WIDTH;//玩家宽度
    int player_height = PLAYER_HEIGHT;//玩家高度
    int shadow_width = SHADOW_WIDTH;//影子宽度
    int player_bullets = 3;//玩家初始子弹数
    int player_health = PALYERHEALTH;//玩家生命值
private:
    IMAGE shadowImg;//影子图片
    Animation* anim_player_left;//玩家左移右移动画
    Animation* anim_player_right;
    FacingState facing = FacingState::Right;//初始化面朝方向
    POINT playerPos = { 500,500 };//初始化位置
    bool isMoveUp = false;
    bool isMoveDown = false;
    bool isMoveLeft = false;
    bool isMoveRight = false;
    int moveSpeedX = 0;//玩家X轴速度
    int moveSpeedY = 0;//玩家Y轴速度
};
#endif
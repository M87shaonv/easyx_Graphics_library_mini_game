#include <string>
#include "Animation.h"

bool running = true;//控制游戏主循环
ExMessage msg;//用于存储从消息队列中检索到的消息
const INT WIDTH = 1280; //屏幕宽度常量
const INT HEIGHT = 720;  //屏幕高度常量
const INT FPS = 144; //帧率常量

#pragma region 动画相关
size_t currentAnimIndex = 0;//当前动画帧索引
const size_t ANIMNUMBER = 6;//动画帧总数常量,动画是循环播放,也就是当动画帧索引到达动画帧总数时,索引重置为0
const INT PLAYER_WIDTH = 80;//玩家宽度常量
const INT PLAYER_HEIGHT = 80;//玩家高度常量
const INT SHADOW_WIDTH = 30;//影子宽度常量
IMAGE bgImg;//背景图片
IMAGE shadowImg;//影子图片
IMAGE playerLeft[ANIMNUMBER];
IMAGE playerRight[ANIMNUMBER];
Animation anim_player_left(_T("img/player_left_%d.png"), 6, 160);//1秒内播放6张图片
Animation anim_player_right(_T("img/player_right_%d.png"), 6, 160);
FacingState facing = FacingState::Right;
#pragma endregion
void DrawPlayer(int delta);
void Init();

POINT playerPos = { 500,500 };//玩家坐标
const INT PLAYERSPEED = 5;//玩家初始初始速度
bool isMoveUp = false;
bool isMoveDown = false;
bool isMoveLeft = false;
bool isMoveRight = false;
int moveSpeedX = 0;//玩家X轴速度
int moveSpeedY = 0;//玩家Y轴速度
int main() {
    //初始化操作
    Init();

    while (running) {
        DWORD startTime = GetTickCount();//获取当前系统时间(ms)
        //DetectionInput();
        while (peekmessage(&msg))//从消息队列中检索消息,不会从队列移除它
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
#pragma region 这种方法移动没有那么快,可以试试
            //if (msg.message == WM_KEYDOWN)
            //switch (msg.vkcode)
            //{
            //case VK_UP:
            //	isMoveUp = true;
            //	break;
            //case VK_DOWN:
            //	isMoveDown = true;
            //	break;
            //case VK_LEFT:
            //	isMoveLeft = true;
            //	break;
            //case VK_RIGHT:
            //	isMoveRight = true;
            //	break;
            //}
#pragma endregion
        }
        moveSpeedX = 0;//玩家X轴速度
        moveSpeedY = 0;//玩家Y轴速度
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

        /*对 2 进行平方根运算，得到约 1.414。这个值是斜着移动时 X 和 Y 轴速度叠加的结果。
        使用 static_cast<int> 将计算结果转换为整数类型，以确保速度是整数值。
        这个调整的过程确保了斜着移动时的总速度在 X 轴和 Y 轴上保持一致*/
        if (moveSpeedX != 0 && moveSpeedY != 0) {
            moveSpeedX = static_cast<int>(moveSpeedX / sqrt(2));
            moveSpeedY = static_cast<int>(moveSpeedY / sqrt(2));
        }
        playerPos.x += moveSpeedX;
        playerPos.y += moveSpeedY;

        //isMoveUp = false;
        //isMoveDown = false;
        //isMoveLeft = false;
        //isMoveRight = false;
        //static size_t counter=0;//记录当前动画帧共播放几个游戏帧,设置为static确保计时器只在第一个游戏帧时初始化为0
        //if (++counter % 5 == 0)//每5个游戏帧切换一个动画帧
        //{
        //	++currentAnimIndex;
        //}
        ////使动画循环播放,使用取模求余数,当ANIMNUMBER==6,结果便为0
        //currentAnimIndex = currentAnimIndex % ANIMNUMBER;
        DWORD endTime = GetTickCount();//获取自系统启动以来所经过的毫秒数
        DWORD deltaTime = endTime - startTime;
        cleardevice();//清除屏幕内容
        putimage(0, 0, &bgImg);//绘制背景
        //putimage_alpha(playerPos.x, playerPos.y, &playerLeft[currentAnimIndex]);
        //DrawPlayer(1000 / 144);//闪电侠速度
        DrawPlayer(20);
        FlushBatchDraw();//将之前缓存图形绘制操作渲染到屏幕上

        /*如果deltaTime小于1000 / 144（约等于6.94毫秒，对应于144Hz的帧率）
        则使用Sleep函数等待一段时间，以确保帧率的稳定*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    EndBatchDraw();//关闭批处理绘制模式
}

/// <summary>
/// 绘制玩家
/// </summary>
/// <param name="delta">绘制间隔</param>
void DrawPlayer(int delta)
{
    //计算了玩家角色宽度的一半和阴影图像宽度的一半之间的差值,以便在玩家角色的正中间绘制阴影图像
    int shadowpos_x = playerPos.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
    //计算了玩家角色高度的差值,以便在玩家角色的底部绘制阴影图像
    int shadowpos_y = playerPos.y + PLAYER_HEIGHT - 10;
    putimage_alpha(shadowpos_x, shadowpos_y, &shadowImg);
    if (facing == FacingState::Left)//如果左移
        anim_player_left.Play(playerPos.x, playerPos.y, delta);
    else//如果右移
        anim_player_right.Play(playerPos.x, playerPos.y, delta);
}
void Init()
{
    initgraph(WIDTH, HEIGHT);//初始化图形窗口
    loadimage(&bgImg, _T("img/background.png"));//加载背景图片
    loadimage(&shadowImg, _T("img/shadow_player.png"));//加载影子图片

    /*开启批处理绘制模式,所有的图形绘制操作缓存起来
    调用FlushBatchDraw时才会被实际渲染到屏幕上。这可以提高绘制的效率*/
    BeginBatchDraw();
}
void DetectionInput() {
}
#pragma region 加载序列帧图片来构成动画的函数,已被Animation类代替
//void LoadAnim()
//{
//	loadimage(&bgImg, _T("img/background.png"));//加载图片
//#pragma region _T宏
//	/*_T是一个宏，它用于在ANSI和Unicode编译环境中提供字符串字面量的兼容性
//	当在ANSI（也称为MBCS，多字节字符集）环境中编译时，_T 宏会将传递给它的字符串作为普通的char*字符串处理
//	但在Unicode（如UTF-16或UTF-32，但Windows通常使用UTF-16）环境中编译代码,例如，当定义了_UNICODE或UNICODE宏时
//	_T 宏会将字符串转换为宽字符字符串（wchar_t*）,可以确保代码可以在这两种环境中无缝工作*/
//#pragma endregion
//	for (size_t i = 0; i < ANIMNUMBER; ++i)
//	{
//		//使用wstring存储宽字符字符串,L前缀表示该字符串字面量是宽字符字符串(wchar_t类型),通常用于支持Unicode字符
//		std::wstring pathl = L"img/player_left_" + std::to_wstring(i) + L".png";
//		loadimage(&playerLeft[i], pathl.c_str());//c_str转换为const wchar_t*类型,C风格宽字符字符串版本
//		std::wstring pathr = L"img/player_right_" + std::to_wstring(i) + L".png";
//		loadimage(&playerRight[i], pathr.c_str());
//	}
//}
#pragma endregion
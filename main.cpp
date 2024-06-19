#include <string>
#include "Player.h"
#include "Config.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
ExMessage msg;//用于存储从消息队列中检索到的消息
bool running = true;//控制游戏主循环
size_t currentAnimIndex = 0;//当前动画帧索引
const size_t ANIMNUMBER = 6;//动画帧总数常量,动画是循环播放,也就是当动画帧索引到达动画帧总数时,索引重置为0

IMAGE bgImg;//背景图片
Player player;//玩家对象
std::vector<Enemy*> enemies;//敌人对象指针容器
void Init();
void GenerateEnemies(std::vector<Enemy*>& enemies);
int main()
{
    //初始化操作
    Init();

    while (running) {
        DWORD startTime = GetTickCount();//获取当前系统时间(ms)
        while (peekmessage(&msg))//从消息队列中检索消息,不会从队列移除它
        {
            player.ProcessEvent(msg);//处理消息
        }
        player.Move();
        GenerateEnemies(enemies);//生成新的敌人
        for (auto& enemy : enemies) {  //遍历敌人对象指针容器来移动
            enemy->Move(player);
        }

        cleardevice();//清除屏幕内容
        putimage(0, 0, &bgImg);//绘制背景

        player.Draw(delta);
        for (auto& enemy : enemies) {  //遍历敌人对象指针容器来绘制
            enemy->Draw(delta);
        }

        FlushBatchDraw();//将之前缓存图形绘制操作渲染到屏幕上

        DWORD endTime = GetTickCount();//获取自系统启动以来所经过的毫秒数
        DWORD deltaTime = endTime - startTime;
        /*如果deltaTime小于1000 / 144（约等于6.94毫秒，对应于144Hz的帧率）
        则使用Sleep函数等待一段时间，以确保帧率的稳定*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    EndBatchDraw();//关闭批处理绘制模式
}

void Init()
{
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);//初始化图形窗口
    loadimage(&bgImg, _T("img/background.png"));//加载背景图片
    /*开启批处理绘制模式,所有的图形绘制操作缓存起来
    调用FlushBatchDraw时才会被实际渲染到屏幕上。这可以提高绘制的效率*/
    BeginBatchDraw();
}
void GenerateEnemies(std::vector<Enemy*>& enemies)//生成新的敌人
{
    const int INTERVAL = 100;//敌人间隔时间(ms)
    static int counter = 0;//计时器
    if (++counter % INTERVAL == 0) {
        enemies.push_back(new Enemy());//生成新的敌人
    }
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
#include <string>
#include "Player.h"
#include "Config.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include "UpgradeOptionFactory.h"

ExMessage msg;//用于存储从消息队列中检索到的消息
bool running = true;//控制游戏主循环
bool ispause = false;//控制游戏暂停
int score = 0;//玩家得分
int   grade = 1;//玩家初始等级
size_t currentAnimIndex = 0;//当前动画帧索引
const size_t ANIMNUMBER = 6;//动画帧总数常量,动画是循环播放,也就是当动画帧索引到达动画帧总数时,索引重置为0

IMAGE bgImg;//背景图片
Player player;//玩家对象
std::vector<Enemy*> enemies;//敌人对象指针容器
std::vector<Bullet> bullets(player.player_bullets);//子弹对象容器
void Init();
void GenerateEnemies(std::vector<Enemy*>& enemies);
void UpdateBullets(std::vector<Bullet>& bullets, const Player& player);
void UpdateExperience(int points);
void DrawExperienceBar();
void EnemysOperations();
void ShowUpgradeOptions(Player& player);
void ShowGameOverMenu();
void ResetGame();
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
        if (ispause) continue; // 如果游戏处于暂停状态,则跳过本次循环
        player.Move();
        UpdateBullets(bullets, player);//更新子弹位置
        GenerateEnemies(enemies);//生成新的敌人
        EnemysOperations();//敌人操作

        cleardevice();//清除屏幕内容
        putimage(0, 0, &bgImg);//绘制背景

        player.Draw(delta);
        for (auto& enemy : enemies)//遍历敌人对象指针容器来绘制
            enemy->Draw(delta);
        for (auto& bullet : bullets)//遍历子弹对象容器来绘制
            bullet.Draw();

        DrawExperienceBar(); // 绘制经验条
        FlushBatchDraw();//将之前缓存图形绘制操作渲染到屏幕上

        DWORD endTime = GetTickCount();//获取自系统启动以来所经过的毫秒数
        DWORD deltaTime = endTime - startTime;
        /*如果deltaTime小于1000 / 144（约等于6.94毫秒，对应于144Hz的帧率）
        则使用Sleep函数等待一段时间，以确保帧率的稳定*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    //游戏结束,释放资源
    delete atlas_player_left;
    delete atlas_player_right;
    delete atlas_enemy_left;
    delete atlas_enemy_right;
    EndBatchDraw();//关闭批处理绘制模式
}

// 更新经验条的逻辑
int experience = 0;//玩家当前经验值
int maxE = maxExperience;//玩家最大经验值
// 修改UpdateExperience函数以显示升级选项
void UpdateExperience(int points)
{
    experience += points;
    if (experience >= maxE) {
        ispause = true; // 暂停游戏
        ShowUpgradeOptions(player); // 显示升级选项
        experience = 0; // 重置经验值
        ++grade;//增加玩家等级
        maxE += maxE * 0.5 * grade;//升级玩家最大经验值
        ispause = false; // 继续游戏
    }
}

// 绘制经验条
void DrawExperienceBar()
{
    // 绘制经验条背景
    setfillcolor(RGB(200, 200, 200));
    solidrectangle(experienceBarX, experienceBarY, experienceBarX + experienceBarWidth, experienceBarY + experienceBarHeight);

    // 绘制经验条进度
    int currentWidth = (experience * experienceBarWidth) / maxE;
    setfillcolor(RGB(0, 255, 0));
    solidrectangle(experienceBarX, experienceBarY, experienceBarX + currentWidth, experienceBarY + experienceBarHeight);

    // 绘制经验值百分比文本
    static TCHAR text[64];
    int percentage = (experience * 100) / maxE;
    _stprintf_s(text, _T("%d%%"), percentage);
    setbkmode(TRANSPARENT); // 设置背景透明
    settextstyle(20, 0, _T("Arial")); // 设置字体
    settextcolor(RGB(0, 0, 0));
    outtextxy(experienceBarX + experienceBarWidth / 2 - 20, experienceBarY + 2, text); // 输出文本
}
void EnemysOperations()
{
    for (auto& enemy : enemies)//遍历敌人对象指针容器来移动
        enemy->Move(player);
    for (auto& enemy : enemies) {//遍历敌人对象指针容器来检查是否与玩家发生碰撞
        if (!enemy->CheckPlayerCollision(player))
            continue;
        static TCHAR text[128];
        _stprintf_s(text, _T("最终得分: %d!"), score);
        ShowGameOverMenu();
        //MessageBoxW(GetHWnd(), text, _T("游戏结束"), MB_OK);
        //running = false;
    }
    for (auto& enemy : enemies) //遍历敌人对象指针容器来检查是否与子弹发生碰撞
        for (const Bullet& bullet : bullets) {
            if (!enemy->CheckBulletCollision(bullet))
                continue;
            mciSendString(_T("play hit from 0"), NULL, 0, NULL);//播放子弹击中音效
            enemy->Hurt();
            ++score;
        }
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy* enemy = enemies[i];
        if (!enemy->CheckAlive()) {
            std::swap(enemies[i], enemies.back()); //将需要移除的敌人放到最后
            enemies.pop_back();//移除最后一个元素
            delete enemy;//释放敌人对象
            UpdateExperience(1);//更新经验值
        }
    }
}
void ShowGameOverMenu()
{
    cleardevice();
    putimage(0, 0, &bgImg); // 绘制背景

    settextstyle(30, 0, _T("Arial"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, _T("游戏结束"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, _T("1. 重新开始"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, _T("2. 退出游戏"));

    FlushBatchDraw();

    int choice = 0;
    while (choice < 1 || choice > 2) {
        if (peekmessage(&msg, EM_KEY)) {
            if (msg.message == WM_KEYDOWN) {
                switch (msg.vkcode) {
                case '1': choice = 1; break;
                case '2': choice = 2; break;
                }
            }
        }
    }

    if (choice == 1) {
        // 重新开始游戏
        ResetGame();
    }
    else if (choice == 2) {
        // 退出游戏
        running = false;
    }
}

void ResetGame()
{
    // 重置游戏状态
    score = 0;
    grade = 1;
    experience = 0;
    maxE = maxExperience;
    ispause = false;

    // 清除现有敌人
    for (auto& enemy : enemies)
        delete enemy;
    enemies.clear();
    // 重置玩家状态
    player.reset();
    // 清除现有子弹
    bullets.clear();
    bullets.resize(player.player_bullets);
    // 重新开始游戏循环
    running = true;
}
/// <summary>
/// 初始化游戏
/// </summary>
void Init()
{
    srand(static_cast<unsigned int>(time(0)));//使用当前时间作为随机数生成器的种子
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);//初始化图形窗口
    loadimage(&bgImg, _T("img/background.png"));//加载背景图片
    /*开启批处理绘制模式,所有的图形绘制操作缓存起来
    调用FlushBatchDraw时才会被实际渲染到屏幕上。这可以提高绘制的效率*/
    BeginBatchDraw();
    /*mciSendString函数是用于发送多媒体命令的 Windows多媒体命令接口（MCI）函数之一
第一个参数是命令字符串
第二个参数是返回讯息的缓冲区指针,对于发送命令时，通常可以传递 NULL
第三个参数是缓冲区的大小，通常可以设置为 0，因为发送命令时不需要返回信息
第四个参数是应用程序的可执行文件名称,Windows都会传递其模块句柄*/
    mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);//打开背景音乐
    mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);//播放背景音乐
    mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);//打开子弹击中音效
}

void GenerateEnemies(std::vector<Enemy*>& enemies)//生成新的敌人
{
    static int counter = 0;//计时器
    if (++counter % (INTERVAL - grade) == 0) {
        enemies.push_back(new Enemy());//生成新的敌人
        if (grade >= 5)
            enemies.push_back(new Enemy());
        if (grade >= 10)
            enemies.push_back(new Enemy());
    }
}

void UpdateBullets(std::vector<Bullet>& bullets, const Player& player)//更新子弹位置
{
    const double RADIAL_VELOCITY = 0.0025;//径向波动速度,决定子弹距离玩家时近时远的波动速度
    const double TANGENTIAL_VELOCITY = 0.0025;//切向波动速度,决定圆周运动的快慢
    double radian_interval = 2 * PI / bullets.size();//子弹之间的弧度间隔
    POINT player_pos = player.GetPosition();
    double radius = 100 + 25 * sin(GetTickCount() * RADIAL_VELOCITY);//子弹距离玩家的半径
    for (size_t i = 0; i < bullets.size(); ++i) {
        double radian = GetTickCount() * TANGENTIAL_VELOCITY + i * radian_interval;//子弹的弧度
        bullets[i].position.x = player_pos.x + player.player_width / 2 + (int)(radius * sin(radian));//子弹的x坐标
        bullets[i].position.y = player_pos.y + player.player_width / 2 + (int)(radius * cos(radian));//子弹的y坐标
    }
}
// 显示升级选项并处理玩家选择
void ShowUpgradeOptions(Player& player)
{
    std::vector<UpgradeOption> options = UpgradeOptionFactory::GenerateUpgradeOptions();

    // 绘制升级选项
    cleardevice();
    putimage(0, 0, &bgImg);//绘制背景
    settextstyle(30, 0, _T("Arial"));
    outtextxy(SCREEN_WIDTH / 2 - 150, 50, _T("选择一个升级选项:"));

    for (size_t i = 0; i < options.size(); ++i) {
        std::wstring text = std::to_wstring(i + 1) + L". " + options[i].name + L"\n" + options[i].description;
        outtextxy(SCREEN_WIDTH / 2 - 150, 100 + i * 100, text.c_str());
    }

    FlushBatchDraw();

    // 等待玩家选择
    int choice = 0;
    while (choice < 1 || choice > 3) {
        if (peekmessage(&msg, EM_KEY)) {
            if (msg.message == WM_KEYDOWN) {
                switch (msg.vkcode) {
                case '1': choice = 1; break;
                case '2': choice = 2; break;
                case '3': choice = 3; break;
                }
            }
        }
    }

    // 应用选择的升级效果
    options[choice - 1].effect(player);
    bullets.resize(player.player_bullets);//调整子弹数量
    player.ResetMove();//重置玩家移动状态
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
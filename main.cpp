#include <string>
#include "Player.h"
#include "Config.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
#include "UpgradeOptionFactory.h"

ExMessage msg;//���ڴ洢����Ϣ�����м���������Ϣ
bool running = true;//������Ϸ��ѭ��
bool ispause = false;//������Ϸ��ͣ
int score = 0;//��ҵ÷�
int   grade = 1;//��ҳ�ʼ�ȼ�
size_t currentAnimIndex = 0;//��ǰ����֡����
const size_t ANIMNUMBER = 6;//����֡��������,������ѭ������,Ҳ���ǵ�����֡�������ﶯ��֡����ʱ,��������Ϊ0

IMAGE bgImg;//����ͼƬ
Player player;//��Ҷ���
std::vector<Enemy*> enemies;//���˶���ָ������
std::vector<Bullet> bullets(player.player_bullets);//�ӵ���������
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
    //��ʼ������
    Init();

    while (running) {
        DWORD startTime = GetTickCount();//��ȡ��ǰϵͳʱ��(ms)
        while (peekmessage(&msg))//����Ϣ�����м�����Ϣ,����Ӷ����Ƴ���
        {
            player.ProcessEvent(msg);//������Ϣ
        }
        if (ispause) continue; // �����Ϸ������ͣ״̬,����������ѭ��
        player.Move();
        UpdateBullets(bullets, player);//�����ӵ�λ��
        GenerateEnemies(enemies);//�����µĵ���
        EnemysOperations();//���˲���

        cleardevice();//�����Ļ����
        putimage(0, 0, &bgImg);//���Ʊ���

        player.Draw(delta);
        for (auto& enemy : enemies)//�������˶���ָ������������
            enemy->Draw(delta);
        for (auto& bullet : bullets)//�����ӵ���������������
            bullet.Draw();

        DrawExperienceBar(); // ���ƾ�����
        FlushBatchDraw();//��֮ǰ����ͼ�λ��Ʋ�����Ⱦ����Ļ��

        DWORD endTime = GetTickCount();//��ȡ��ϵͳ���������������ĺ�����
        DWORD deltaTime = endTime - startTime;
        /*���deltaTimeС��1000 / 144��Լ����6.94���룬��Ӧ��144Hz��֡�ʣ�
        ��ʹ��Sleep�����ȴ�һ��ʱ�䣬��ȷ��֡�ʵ��ȶ�*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    //��Ϸ����,�ͷ���Դ
    delete atlas_player_left;
    delete atlas_player_right;
    delete atlas_enemy_left;
    delete atlas_enemy_right;
    EndBatchDraw();//�ر����������ģʽ
}

// ���¾��������߼�
int experience = 0;//��ҵ�ǰ����ֵ
int maxE = maxExperience;//��������ֵ
// �޸�UpdateExperience��������ʾ����ѡ��
void UpdateExperience(int points)
{
    experience += points;
    if (experience >= maxE) {
        ispause = true; // ��ͣ��Ϸ
        ShowUpgradeOptions(player); // ��ʾ����ѡ��
        experience = 0; // ���þ���ֵ
        ++grade;//������ҵȼ�
        maxE += maxE * 0.5 * grade;//������������ֵ
        ispause = false; // ������Ϸ
    }
}

// ���ƾ�����
void DrawExperienceBar()
{
    // ���ƾ���������
    setfillcolor(RGB(200, 200, 200));
    solidrectangle(experienceBarX, experienceBarY, experienceBarX + experienceBarWidth, experienceBarY + experienceBarHeight);

    // ���ƾ���������
    int currentWidth = (experience * experienceBarWidth) / maxE;
    setfillcolor(RGB(0, 255, 0));
    solidrectangle(experienceBarX, experienceBarY, experienceBarX + currentWidth, experienceBarY + experienceBarHeight);

    // ���ƾ���ֵ�ٷֱ��ı�
    static TCHAR text[64];
    int percentage = (experience * 100) / maxE;
    _stprintf_s(text, _T("%d%%"), percentage);
    setbkmode(TRANSPARENT); // ���ñ���͸��
    settextstyle(20, 0, _T("Arial")); // ��������
    settextcolor(RGB(0, 0, 0));
    outtextxy(experienceBarX + experienceBarWidth / 2 - 20, experienceBarY + 2, text); // ����ı�
}
void EnemysOperations()
{
    for (auto& enemy : enemies)//�������˶���ָ���������ƶ�
        enemy->Move(player);
    for (auto& enemy : enemies) {//�������˶���ָ������������Ƿ�����ҷ�����ײ
        if (!enemy->CheckPlayerCollision(player))
            continue;
        static TCHAR text[128];
        _stprintf_s(text, _T("���յ÷�: %d!"), score);
        ShowGameOverMenu();
        //MessageBoxW(GetHWnd(), text, _T("��Ϸ����"), MB_OK);
        //running = false;
    }
    for (auto& enemy : enemies) //�������˶���ָ������������Ƿ����ӵ�������ײ
        for (const Bullet& bullet : bullets) {
            if (!enemy->CheckBulletCollision(bullet))
                continue;
            mciSendString(_T("play hit from 0"), NULL, 0, NULL);//�����ӵ�������Ч
            enemy->Hurt();
            ++score;
        }
    for (size_t i = 0; i < enemies.size(); ++i) {
        Enemy* enemy = enemies[i];
        if (!enemy->CheckAlive()) {
            std::swap(enemies[i], enemies.back()); //����Ҫ�Ƴ��ĵ��˷ŵ����
            enemies.pop_back();//�Ƴ����һ��Ԫ��
            delete enemy;//�ͷŵ��˶���
            UpdateExperience(1);//���¾���ֵ
        }
    }
}
void ShowGameOverMenu()
{
    cleardevice();
    putimage(0, 0, &bgImg); // ���Ʊ���

    settextstyle(30, 0, _T("Arial"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 100, _T("��Ϸ����"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2 - 50, _T("1. ���¿�ʼ"));
    outtextxy(SCREEN_WIDTH / 2 - 150, SCREEN_HEIGHT / 2, _T("2. �˳���Ϸ"));

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
        // ���¿�ʼ��Ϸ
        ResetGame();
    }
    else if (choice == 2) {
        // �˳���Ϸ
        running = false;
    }
}

void ResetGame()
{
    // ������Ϸ״̬
    score = 0;
    grade = 1;
    experience = 0;
    maxE = maxExperience;
    ispause = false;

    // ������е���
    for (auto& enemy : enemies)
        delete enemy;
    enemies.clear();
    // �������״̬
    player.reset();
    // ��������ӵ�
    bullets.clear();
    bullets.resize(player.player_bullets);
    // ���¿�ʼ��Ϸѭ��
    running = true;
}
/// <summary>
/// ��ʼ����Ϸ
/// </summary>
void Init()
{
    srand(static_cast<unsigned int>(time(0)));//ʹ�õ�ǰʱ����Ϊ�����������������
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);//��ʼ��ͼ�δ���
    loadimage(&bgImg, _T("img/background.png"));//���ر���ͼƬ
    /*�������������ģʽ,���е�ͼ�λ��Ʋ�����������
    ����FlushBatchDrawʱ�Żᱻʵ����Ⱦ����Ļ�ϡ��������߻��Ƶ�Ч��*/
    BeginBatchDraw();
    /*mciSendString���������ڷ��Ͷ�ý������� Windows��ý������ӿڣ�MCI������֮һ
��һ�������������ַ���
�ڶ��������Ƿ���ѶϢ�Ļ�����ָ��,���ڷ�������ʱ��ͨ�����Դ��� NULL
�����������ǻ������Ĵ�С��ͨ����������Ϊ 0����Ϊ��������ʱ����Ҫ������Ϣ
���ĸ�������Ӧ�ó���Ŀ�ִ���ļ�����,Windows���ᴫ����ģ����*/
    mciSendString(_T("open mus/bgm.mp3 alias bgm"), NULL, 0, NULL);//�򿪱�������
    mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);//���ű�������
    mciSendString(_T("open mus/hit.wav alias hit"), NULL, 0, NULL);//���ӵ�������Ч
}

void GenerateEnemies(std::vector<Enemy*>& enemies)//�����µĵ���
{
    static int counter = 0;//��ʱ��
    if (++counter % (INTERVAL - grade) == 0) {
        enemies.push_back(new Enemy());//�����µĵ���
        if (grade >= 5)
            enemies.push_back(new Enemy());
        if (grade >= 10)
            enemies.push_back(new Enemy());
    }
}

void UpdateBullets(std::vector<Bullet>& bullets, const Player& player)//�����ӵ�λ��
{
    const double RADIAL_VELOCITY = 0.0025;//���򲨶��ٶ�,�����ӵ��������ʱ��ʱԶ�Ĳ����ٶ�
    const double TANGENTIAL_VELOCITY = 0.0025;//���򲨶��ٶ�,����Բ���˶��Ŀ���
    double radian_interval = 2 * PI / bullets.size();//�ӵ�֮��Ļ��ȼ��
    POINT player_pos = player.GetPosition();
    double radius = 100 + 25 * sin(GetTickCount() * RADIAL_VELOCITY);//�ӵ�������ҵİ뾶
    for (size_t i = 0; i < bullets.size(); ++i) {
        double radian = GetTickCount() * TANGENTIAL_VELOCITY + i * radian_interval;//�ӵ��Ļ���
        bullets[i].position.x = player_pos.x + player.player_width / 2 + (int)(radius * sin(radian));//�ӵ���x����
        bullets[i].position.y = player_pos.y + player.player_width / 2 + (int)(radius * cos(radian));//�ӵ���y����
    }
}
// ��ʾ����ѡ��������ѡ��
void ShowUpgradeOptions(Player& player)
{
    std::vector<UpgradeOption> options = UpgradeOptionFactory::GenerateUpgradeOptions();

    // ��������ѡ��
    cleardevice();
    putimage(0, 0, &bgImg);//���Ʊ���
    settextstyle(30, 0, _T("Arial"));
    outtextxy(SCREEN_WIDTH / 2 - 150, 50, _T("ѡ��һ������ѡ��:"));

    for (size_t i = 0; i < options.size(); ++i) {
        std::wstring text = std::to_wstring(i + 1) + L". " + options[i].name + L"\n" + options[i].description;
        outtextxy(SCREEN_WIDTH / 2 - 150, 100 + i * 100, text.c_str());
    }

    FlushBatchDraw();

    // �ȴ����ѡ��
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

    // Ӧ��ѡ�������Ч��
    options[choice - 1].effect(player);
    bullets.resize(player.player_bullets);//�����ӵ�����
    player.ResetMove();//��������ƶ�״̬
}

#pragma region ��������֡ͼƬ�����ɶ����ĺ���,�ѱ�Animation�����
//void LoadAnim()
//{
//	loadimage(&bgImg, _T("img/background.png"));//����ͼƬ
//#pragma region _T��
//	/*_T��һ���꣬��������ANSI��Unicode���뻷�����ṩ�ַ����������ļ�����
//	����ANSI��Ҳ��ΪMBCS�����ֽ��ַ����������б���ʱ��_T ��Ὣ���ݸ������ַ�����Ϊ��ͨ��char*�ַ�������
//	����Unicode����UTF-16��UTF-32����Windowsͨ��ʹ��UTF-16�������б������,���磬��������_UNICODE��UNICODE��ʱ
//	_T ��Ὣ�ַ���ת��Ϊ���ַ��ַ�����wchar_t*��,����ȷ����������������ֻ������޷칤��*/
//#pragma endregion
//	for (size_t i = 0; i < ANIMNUMBER; ++i)
//	{
//		//ʹ��wstring�洢���ַ��ַ���,Lǰ׺��ʾ���ַ����������ǿ��ַ��ַ���(wchar_t����),ͨ������֧��Unicode�ַ�
//		std::wstring pathl = L"img/player_left_" + std::to_wstring(i) + L".png";
//		loadimage(&playerLeft[i], pathl.c_str());//c_strת��Ϊconst wchar_t*����,C�����ַ��ַ����汾
//		std::wstring pathr = L"img/player_right_" + std::to_wstring(i) + L".png";
//		loadimage(&playerRight[i], pathr.c_str());
//	}
//}
#pragma endregion
#include <string>
#include "Player.h"
#include "Config.h"
#include "Enemy.h"
#include "Bullet.h"
#include <iostream>
ExMessage msg;//���ڴ洢����Ϣ�����м���������Ϣ
bool running = true;//������Ϸ��ѭ��
size_t currentAnimIndex = 0;//��ǰ����֡����
const size_t ANIMNUMBER = 6;//����֡��������,������ѭ������,Ҳ���ǵ�����֡�������ﶯ��֡����ʱ,��������Ϊ0

IMAGE bgImg;//����ͼƬ
Player player;//��Ҷ���
std::vector<Enemy*> enemies;//���˶���ָ������
void Init();
void GenerateEnemies(std::vector<Enemy*>& enemies);
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
        player.Move();
        GenerateEnemies(enemies);//�����µĵ���
        for (auto& enemy : enemies) {  //�������˶���ָ���������ƶ�
            enemy->Move(player);
        }

        cleardevice();//�����Ļ����
        putimage(0, 0, &bgImg);//���Ʊ���

        player.Draw(delta);
        for (auto& enemy : enemies) {  //�������˶���ָ������������
            enemy->Draw(delta);
        }

        FlushBatchDraw();//��֮ǰ����ͼ�λ��Ʋ�����Ⱦ����Ļ��

        DWORD endTime = GetTickCount();//��ȡ��ϵͳ���������������ĺ�����
        DWORD deltaTime = endTime - startTime;
        /*���deltaTimeС��1000 / 144��Լ����6.94���룬��Ӧ��144Hz��֡�ʣ�
        ��ʹ��Sleep�����ȴ�һ��ʱ�䣬��ȷ��֡�ʵ��ȶ�*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    EndBatchDraw();//�ر����������ģʽ
}

void Init()
{
    initgraph(SCREEN_WIDTH, SCREEN_HEIGHT);//��ʼ��ͼ�δ���
    loadimage(&bgImg, _T("img/background.png"));//���ر���ͼƬ
    /*�������������ģʽ,���е�ͼ�λ��Ʋ�����������
    ����FlushBatchDrawʱ�Żᱻʵ����Ⱦ����Ļ�ϡ��������߻��Ƶ�Ч��*/
    BeginBatchDraw();
}
void GenerateEnemies(std::vector<Enemy*>& enemies)//�����µĵ���
{
    const int INTERVAL = 100;//���˼��ʱ��(ms)
    static int counter = 0;//��ʱ��
    if (++counter % INTERVAL == 0) {
        enemies.push_back(new Enemy());//�����µĵ���
    }
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
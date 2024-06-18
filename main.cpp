#include <string>
#include "Animation.h"

bool running = true;//������Ϸ��ѭ��
ExMessage msg;//���ڴ洢����Ϣ�����м���������Ϣ
const INT WIDTH = 1280; //��Ļ��ȳ���
const INT HEIGHT = 720;  //��Ļ�߶ȳ���
const INT FPS = 144; //֡�ʳ���

#pragma region �������
size_t currentAnimIndex = 0;//��ǰ����֡����
const size_t ANIMNUMBER = 6;//����֡��������,������ѭ������,Ҳ���ǵ�����֡�������ﶯ��֡����ʱ,��������Ϊ0
const INT PLAYER_WIDTH = 80;//��ҿ�ȳ���
const INT PLAYER_HEIGHT = 80;//��Ҹ߶ȳ���
const INT SHADOW_WIDTH = 30;//Ӱ�ӿ�ȳ���
IMAGE bgImg;//����ͼƬ
IMAGE shadowImg;//Ӱ��ͼƬ
IMAGE playerLeft[ANIMNUMBER];
IMAGE playerRight[ANIMNUMBER];
Animation anim_player_left(_T("img/player_left_%d.png"), 6, 160);//1���ڲ���6��ͼƬ
Animation anim_player_right(_T("img/player_right_%d.png"), 6, 160);
FacingState facing = FacingState::Right;
#pragma endregion
void DrawPlayer(int delta);
void Init();

POINT playerPos = { 500,500 };//�������
const INT PLAYERSPEED = 5;//��ҳ�ʼ��ʼ�ٶ�
bool isMoveUp = false;
bool isMoveDown = false;
bool isMoveLeft = false;
bool isMoveRight = false;
int moveSpeedX = 0;//���X���ٶ�
int moveSpeedY = 0;//���Y���ٶ�
int main() {
    //��ʼ������
    Init();

    while (running) {
        DWORD startTime = GetTickCount();//��ȡ��ǰϵͳʱ��(ms)
        //DetectionInput();
        while (peekmessage(&msg))//����Ϣ�����м�����Ϣ,����Ӷ����Ƴ���
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
#pragma region ���ַ����ƶ�û����ô��,��������
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

        //isMoveUp = false;
        //isMoveDown = false;
        //isMoveLeft = false;
        //isMoveRight = false;
        //static size_t counter=0;//��¼��ǰ����֡�����ż�����Ϸ֡,����Ϊstaticȷ����ʱ��ֻ�ڵ�һ����Ϸ֡ʱ��ʼ��Ϊ0
        //if (++counter % 5 == 0)//ÿ5����Ϸ֡�л�һ������֡
        //{
        //	++currentAnimIndex;
        //}
        ////ʹ����ѭ������,ʹ��ȡģ������,��ANIMNUMBER==6,�����Ϊ0
        //currentAnimIndex = currentAnimIndex % ANIMNUMBER;
        DWORD endTime = GetTickCount();//��ȡ��ϵͳ���������������ĺ�����
        DWORD deltaTime = endTime - startTime;
        cleardevice();//�����Ļ����
        putimage(0, 0, &bgImg);//���Ʊ���
        //putimage_alpha(playerPos.x, playerPos.y, &playerLeft[currentAnimIndex]);
        //DrawPlayer(1000 / 144);//�������ٶ�
        DrawPlayer(20);
        FlushBatchDraw();//��֮ǰ����ͼ�λ��Ʋ�����Ⱦ����Ļ��

        /*���deltaTimeС��1000 / 144��Լ����6.94���룬��Ӧ��144Hz��֡�ʣ�
        ��ʹ��Sleep�����ȴ�һ��ʱ�䣬��ȷ��֡�ʵ��ȶ�*/
        if (deltaTime < 1000 / FPS)
            Sleep(1000 / 144 - deltaTime);
    }
    EndBatchDraw();//�ر����������ģʽ
}

/// <summary>
/// �������
/// </summary>
/// <param name="delta">���Ƽ��</param>
void DrawPlayer(int delta)
{
    //��������ҽ�ɫ��ȵ�һ�����Ӱͼ���ȵ�һ��֮��Ĳ�ֵ,�Ա�����ҽ�ɫ�����м������Ӱͼ��
    int shadowpos_x = playerPos.x + (PLAYER_WIDTH / 2 - SHADOW_WIDTH / 2);
    //��������ҽ�ɫ�߶ȵĲ�ֵ,�Ա�����ҽ�ɫ�ĵײ�������Ӱͼ��
    int shadowpos_y = playerPos.y + PLAYER_HEIGHT - 10;
    putimage_alpha(shadowpos_x, shadowpos_y, &shadowImg);
    if (facing == FacingState::Left)//�������
        anim_player_left.Play(playerPos.x, playerPos.y, delta);
    else//�������
        anim_player_right.Play(playerPos.x, playerPos.y, delta);
}
void Init()
{
    initgraph(WIDTH, HEIGHT);//��ʼ��ͼ�δ���
    loadimage(&bgImg, _T("img/background.png"));//���ر���ͼƬ
    loadimage(&shadowImg, _T("img/shadow_player.png"));//����Ӱ��ͼƬ

    /*�������������ģʽ,���е�ͼ�λ��Ʋ�����������
    ����FlushBatchDrawʱ�Żᱻʵ����Ⱦ����Ļ�ϡ��������߻��Ƶ�Ч��*/
    BeginBatchDraw();
}
void DetectionInput() {
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
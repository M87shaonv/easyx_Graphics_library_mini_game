#ifndef CONFIG_H
#define CONFIG_H
//����Ϊconstexpr,�Ϳ����ظ�����ͷ�ļ������õ����ظ�����
typedef int INT;
constexpr   INT SCREEN_WIDTH = 1280; //��Ļ��ȳ���
constexpr    INT SCREEN_HEIGHT = 720;  //��Ļ�߶ȳ���
constexpr    INT FPS = 144; //֡�ʳ���
constexpr   INT delta = 1000 / FPS; //�����л�ʱ��������,��ֵԽ��,�����л����ٶ�Խ��
#endif

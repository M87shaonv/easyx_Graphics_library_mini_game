#ifndef CONFIG_H
#define CONFIG_H
//����Ϊconstexpr,�Ϳ����ظ�����ͷ�ļ������õ����ظ�����
constexpr   INT SCREEN_WIDTH = 1280; //��Ļ��ȳ���
constexpr    INT SCREEN_HEIGHT = 720;  //��Ļ�߶ȳ���
constexpr    INT FPS = 120; //֡�ʳ���
constexpr   INT delta = 1000 / FPS; //�����л�ʱ��������,��ֵԽ��,�����л����ٶ�Խ��
constexpr DOUBLE PI = 3.14159;
// ���徭���������Ժ�״̬

constexpr int maxExperience = 10;//�����ֵ
constexpr int experienceBarWidth = 600;//���������
constexpr int experienceBarHeight = 20;//�������߶�
constexpr int experienceBarX = (SCREEN_WIDTH - experienceBarWidth) / 2;//������X����
constexpr int experienceBarY = 10; //������Y����
constexpr int INTERVAL = 100;//���˼��ʱ��(ms)
#endif

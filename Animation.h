#ifndef ANIMATION_H
#define ANIMATION_H
#include <graphics.h>
#include <vector>

#pragma comment(lib,"MSIMG32.LIB")//Ԥ����ָ��,���߱���������ʱ�����ÿ�,�ṩAlphaBlend����
//��������,ʹ��IMGAE�����͸������Ϣ�����Ƶ���Ļ.���ʹ��putimage,�زĻ��кڱ�
inline void putimage_alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();//��ȡͼ����
    int h = img->getheight();
    /*WindowsGDI����,���ڴ���alpha��ϵ�ͼ�����
    ��һ��������ȡͼ���豸���,Ҳ������ʾ��
    x/y�ǻ���ͼ���Ŀ��λ��
    w/h�ǻ���ͼ��Ŀ�Ⱥ͸߶�
    Ȼ���ȡͼ���HDC(�豸������)������Ա�AlphaBlend�������Դ��ж�ȡ��������*/
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
#pragma region ���
    /*�����һ�����ڱ�ʶ������ϵͳ��Դ�����ڴ�顢�ļ����豸�����ĵȣ�������ֵ
    ��Windows�У�������ɲ���ϵͳ���䲢����ģ�����ͨ���ǲ�͸����
    ������Ա����ҪҲ����ֱ�ӷ��ʾ����ָ����ڴ����������
    ����ԱӦ��ʹ��Windows API������ͨ�������ӵط��ʺͲ�����Դ*/
#pragma endregion
}

//����FacingStateö������,���ڱ�ʾ��ɫ�ĳ���״̬
enum FacingState
{
    Left,
    Right,
    Move,
};

class Animation
{
public:
    /*LPCTCH��һ��Windowsƽ̨������ָ�����ı��ַ���ָ�����Ͷ���
    * num�Ƕ���֡����
    interval��ʾÿ֡����ʱ��,��λΪ����*/
    Animation(LPCTCH path, int num, int interval)
    {
        interval_ms = interval;
        TCHAR filepath[256];
        for (size_t i = 0; i < num; ++i)
        {
            _stprintf_s(filepath, path, i);//ʹ��_stprintf_s����������iת��Ϊ�ַ���,��ƴ�ӵ�path��
            IMAGE* frame = new IMAGE();//��̬����IMAGE������ڴ�,��ַ��ֵ��ָ��
            loadimage(frame, filepath);//����ͼ��frame��
            frame_sequence.push_back(frame);//��frame��ӵ�������
            /*IMAGE����(ʵ��)���ڶ��Ϸ���ģ���������������ڲ������žֲ�ָ�����frame����ʧ������
            ���ʵ����һֱ���ڣ�ֱ����ʽ��ʹ��delete���ͷ���*/
        }
    }
    ~Animation()
    {
        for (size_t i = 0; i < frame_sequence.size(); ++i)
            delete frame_sequence[i];//�ͷŶ�̬�����IMAGE����
    }

    void Play(int x, int y, int delta)//delta��ʾ�����ϴε���play��ȥ���
    {
        timer += delta;//�ۼӼ�ʱ��
        if (timer >= interval_ms)//�����ʱ��������֡���,���л�����һ֡
        {
            //����֡����
            frame_index = ++frame_index % frame_sequence.size();
            timer = 0;
        }
        putimage_alpha(x, y, frame_sequence[frame_index]);//��ʾ��ǰ֡
    }

private:
    int timer = 0;//������ʱ��
    int frame_index = 0;//����֡����
    int interval_ms = 0;//֡���
    std::vector<IMAGE*> frame_sequence;//�洢֡����
};
#endif // !ANIMATION
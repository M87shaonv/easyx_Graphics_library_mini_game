#ifndef ANIMATION_H
#define ANIMATION_H
#include <graphics.h>
#include <vector>

#pragma comment(lib,"MSIMG32.LIB")//预处理指令,告诉编译器链接时包含该库,提供AlphaBlend函数
//内联函数,使用IMGAE对象的透明度信息来绘制到屏幕.如果使用putimage,素材会有黑边
inline void putimage_alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();//获取图像宽高
    int h = img->getheight();
    /*WindowsGDI函数,用于带有alpha混合的图像绘制
    第一个参数获取图形设备句柄,也就是显示屏
    x/y是绘制图像的目标位置
    w/h是绘制图像的宽度和高度
    然后获取图像的HDC(设备上下文)句柄，以便AlphaBlend函数可以从中读取像素数据*/
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
#pragma region 句柄
    /*句柄是一个用于标识和引用系统资源（如内存块、文件、设备上下文等）的整数值
    在Windows中，句柄是由操作系统分配并管理的，并且通常是不透明的
    即程序员不需要也不能直接访问句柄所指向的内存区域的内容
    程序员应该使用Windows API函数来通过句柄间接地访问和操作资源*/
#pragma endregion
}

//定义FacingState枚举类型,用于表示角色的朝向状态
enum FacingState
{
    Left,
    Right,
    Move,
};

class Animation
{
public:
    /*LPCTCH是一个Windows平台上用于指向常量文本字符的指针类型定义
    * num是动画帧总数
    interval表示每帧持续时间,单位为毫秒*/
    Animation(LPCTCH path, int num, int interval)
    {
        interval_ms = interval;
        TCHAR filepath[256];
        for (size_t i = 0; i < num; ++i)
        {
            _stprintf_s(filepath, path, i);//使用_stprintf_s函数将数字i转换为字符串,并拼接到path中
            IMAGE* frame = new IMAGE();//动态分配IMAGE对象的内存,地址赋值给指针
            loadimage(frame, filepath);//加载图像到frame中
            frame_sequence.push_back(frame);//将frame添加到容器中
            /*IMAGE对象(实例)是在堆上分配的，因此它的生命周期不会随着局部指针变量frame的消失而结束
            这个实例会一直存在，直到显式地使用delete来释放它*/
        }
    }
    ~Animation()
    {
        for (size_t i = 0; i < frame_sequence.size(); ++i)
            delete frame_sequence[i];//释放动态分配的IMAGE对象
    }

    void Play(int x, int y, int delta)//delta表示距离上次调用play过去多久
    {
        timer += delta;//累加计时器
        if (timer >= interval_ms)//如果计时器超过了帧间隔,则切换到下一帧
        {
            //更新帧索引
            frame_index = ++frame_index % frame_sequence.size();
            timer = 0;
        }
        putimage_alpha(x, y, frame_sequence[frame_index]);//显示当前帧
    }

private:
    int timer = 0;//动画计时器
    int frame_index = 0;//动画帧索引
    int interval_ms = 0;//帧间隔
    std::vector<IMAGE*> frame_sequence;//存储帧序列
};
#endif // !ANIMATION
#ifndef ATLAS_H
#define ATLAS_H

#include <graphics.h>
#include <vector>

#pragma comment(lib,"MSIMG32.LIB")
#pragma comment(lib,"Winmm.lib")

inline void putimage_alpha(int x, int y, IMAGE* img)
{
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h,
        GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

class Atlas
{
public:
    Atlas(LPCTSTR path, int num)
    {
        TCHAR filepath[256];
        for (size_t i = 0; i < num; ++i)
        {
            _stprintf_s(filepath, path, i);
            IMAGE* frame = new IMAGE();
            loadimage(frame, filepath);
            frame_sequence.push_back(frame);
        }
    }
    ~Atlas()
    {
        for (size_t i = 0; i < frame_sequence.size(); ++i)
            delete frame_sequence[i];
    }

public:
    std::vector<IMAGE*> frame_sequence;
};

// 定义全局指针变量
extern Atlas* atlas_player_left;
extern Atlas* atlas_player_right;
extern Atlas* atlas_enemy_left;
extern Atlas* atlas_enemy_right;
extern Atlas* atlas_autoenemy_idle;
extern Atlas* atlas_autoenemy_blast;
#endif // ATLAS_H

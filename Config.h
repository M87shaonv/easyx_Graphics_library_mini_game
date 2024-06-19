#ifndef CONFIG_H
#define CONFIG_H
//设置为constexpr,就可以重复包含头文件而不用担心重复定义
typedef int INT;
constexpr   INT SCREEN_WIDTH = 1280; //屏幕宽度常量
constexpr    INT SCREEN_HEIGHT = 720;  //屏幕高度常量
constexpr    INT FPS = 144; //帧率常量
constexpr   INT delta = 1000 / FPS; //动画切换时间间隔常量,该值越高,动画切换的速度越快
#endif

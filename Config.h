#ifndef CONFIG_H
#define CONFIG_H
//设置为constexpr,就可以重复包含头文件而不用担心重复定义
constexpr   INT SCREEN_WIDTH = 1280; //屏幕宽度常量
constexpr    INT SCREEN_HEIGHT = 720;  //屏幕高度常量
constexpr    INT FPS = 120; //帧率常量
constexpr   INT delta = 1000 / FPS; //动画切换时间间隔常量,该值越高,动画切换的速度越快
constexpr DOUBLE PI = 3.14159;
// 定义经验条的属性和状态

constexpr int maxExperience = 10;//最大经验值
constexpr int experienceBarWidth = 600;//经验条宽度
constexpr int experienceBarHeight = 20;//经验条高度
constexpr int experienceBarX = (SCREEN_WIDTH - experienceBarWidth) / 2;//经验条X坐标
constexpr int experienceBarY = 10; //经验条Y坐标
constexpr int INTERVAL = 100;//敌人间隔时间(ms)
#endif

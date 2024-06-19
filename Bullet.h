#ifndef BULLET_H
#define BULLET_H
#include <graphics.h>
class Bullet
{
public:
    Bullet();
    ~Bullet();
    //如果const放在函数参数列表后面，这表示该成员函数不会修改类的任何成员变量
    void draw() const;
public:
    POINT position;
private:
    const int RADIUS = 5;//子弹半径
};
#endif // BULLET_H

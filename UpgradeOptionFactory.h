#ifndef UPGRADEOPTIONFACTORY_H
#define UPGRADEOPTIONFACTORY_H
#include <string>
#include <vector>
#include <random>
#include "Player.h"
// 定义升级选项结构体
struct UpgradeOption
{
    std::wstring name;
    std::wstring description;
    void (*effect)(Player&); // 指向升级效果函数的指针
};
// 升级选项工厂类
class UpgradeOptionFactory
{
public:
    static std::vector<UpgradeOption> GenerateUpgradeOptions()
    {
        std::vector<UpgradeOption> options;
        options.push_back(UpgradeOption(L"增加速度", L"提高1点移动速度", &IncreaseSpeed));
        options.push_back(UpgradeOption(L"增加子弹", L"增加1颗子弹", &IncreaseBullets));
        // 随机打乱选项
        std::srand(std::time(0));
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(options.begin(), options.end(), g);

        return options;
    }
    // 升级效果函数
    static void IncreaseSpeed(Player& player)
    {
        player.playerspeed += 1;
    }
    static void IncreaseBullets(Player& player)
    {
        player.player_bullets += 1;
    }
};

#endif // UPGRADEOPTIONFACTORY_H

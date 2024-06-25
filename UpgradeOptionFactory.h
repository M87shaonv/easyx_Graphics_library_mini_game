#ifndef UPGRADEOPTIONFACTORY_H
#define UPGRADEOPTIONFACTORY_H
#include <string>
#include <vector>
#include <random>
#include "Player.h"
// ��������ѡ��ṹ��
struct UpgradeOption
{
    std::wstring name;
    std::wstring description;
    void (*effect)(Player&); // ָ������Ч��������ָ��
};
// ����ѡ�����
class UpgradeOptionFactory
{
public:
    static std::vector<UpgradeOption> GenerateUpgradeOptions()
    {
        std::vector<UpgradeOption> options;
        options.push_back(UpgradeOption(L"�����ٶ�", L"���1���ƶ��ٶ�", &IncreaseSpeed));
        options.push_back(UpgradeOption(L"�����ӵ�", L"����1���ӵ�", &IncreaseBullets));
        // �������ѡ��
        std::srand(std::time(0));
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(options.begin(), options.end(), g);

        return options;
    }
    // ����Ч������
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

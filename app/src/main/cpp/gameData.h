#pragma once

#include "bubble.h"
#include <vector>
class GameData {
private:
    GameData();
    ~GameData();

    static GameData& Get()
    {
        static GameData instance;
        return instance;
    }

    float m_SpawnInterval = 1.5f;
    float m_MinSpawnInterval = 0.20f;

    std::vector<int> m_ComboCount = std::vector<int>(3, 0); // Indexed by BubbleType
    bool m_ElectroShieldActive = false;
    float m_ShieldDuration = 5.0f;
    float m_ElectroShieldTimer = 0.0f;

    void UpdateInternal(float dT);

    void SetSpawnIntervalInternal(float interval);

    void DecreaseSpawnIntervalInternal(float factor);

    void AddSpecialBubbleInternal(BubbleType type);

    void ActivateAnemoBlast();

public:
    GameData(GameData&&) = delete;
    GameData(const GameData&) = delete;
    GameData& operator=(GameData&&) = delete;
    GameData& operator=(const GameData&) = delete;

    static float GetSpawnInterval() { return Get().m_SpawnInterval; }

    static void SetSpawnInterval(float interval) { Get().SetSpawnIntervalInternal(interval); }

    static void DecreaseSpawnInterval(float factor) { Get().DecreaseSpawnIntervalInternal(factor); }

    static void AddSpecialBubble(BubbleType type) { Get().AddSpecialBubbleInternal(type); }

    static bool ElectroShieldActive() { return Get().m_ElectroShieldActive; }

    static void Update(float dT) { Get().UpdateInternal(dT); }
};

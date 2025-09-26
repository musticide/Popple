#pragma once

#include "bubble.h"
#include <vector>
#define MAX_COMBO_LENGTH 3

enum ElementalEffect { NO_ELEMENTAL_EFFECT = 0, ELECTRO, ANEMO, ELEMENTAL_EFFECT_COUNT };

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

    std::vector<int> m_ComboCount = std::vector<int>(BUBBLE_TYPES_COUNT, 0); // Indexed by BubbleType
    // bool m_ElectroShieldActive = false;
    float m_ShieldDuration = 5.0f;
    float m_ElectroShieldTimer = 0.0f;

    ElementalEffect m_ActiveElementalEffect;

    void UpdateInternal(float dT);

    void SetSpawnIntervalInternal(float interval);

    void DecreaseSpawnIntervalInternal(float factor);

    void AddSpecialBubbleInternal(BubbleType type);

    void ActivateAnemoBlast();

    void DrawComboCountInternal();

    Color GetElementalColorInternal(ElementalEffect type);

public:
    GameData(GameData&&) = delete;
    GameData(const GameData&) = delete;
    GameData& operator=(GameData&&) = delete;
    GameData& operator=(const GameData&) = delete;

    static float GetSpawnInterval() { return Get().m_SpawnInterval; }

    static void SetSpawnInterval(float interval) { Get().SetSpawnIntervalInternal(interval); }

    static void DecreaseSpawnInterval(float factor) { Get().DecreaseSpawnIntervalInternal(factor); }

    static void AddSpecialBubble(BubbleType type) { Get().AddSpecialBubbleInternal(type); }

    // static bool ElectroShieldActive() { return Get().m_ElectroShieldActive; }

    static ElementalEffect GetActiveElementalEffect() { return Get().m_ActiveElementalEffect; }

    static void DrawComboCount() { Get().DrawComboCountInternal(); }

    static void Update(float dT) { Get().UpdateInternal(dT); }

    static Color GetElementalColor(ElementalEffect type) { return Get().GetElementalColorInternal(type); }
};

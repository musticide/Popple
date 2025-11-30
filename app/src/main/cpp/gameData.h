#pragma once

#include "Entity.h"
#include "Singleton.h"
#include "bubble.h"
#include <vector>

enum ElementalEffect { NO_ELEMENTAL_EFFECT = 0, ELECTRO, ANEMO, ELEMENTAL_EFFECT_COUNT };

class GameData : public Entity, public Singleton<GameData>{
private:

    float m_SpawnInterval = 1.5f;
    float m_MinSpawnInterval = 0.20f;

    std::vector<int> m_ComboCount = std::vector<int>(BUBBLE_TYPES_COUNT, 0); // Indexed by BubbleType
    ElementalEffect m_ActiveElementalEffect;

    float m_ShieldDuration = 5.0f;
    float m_ElectroShieldTimer = 0.0f;

    float m_AnemoEffectTimer = 0.0f;
    float m_AnemoEffectDuration = 1.0f;


    void SetSpawnIntervalInternal(float interval);

    void DecreaseSpawnIntervalInternal(float factor);

    void AddSpecialBubbleInternal(BubbleType type);

    void ActivateAnemoBlast();

    void DrawComboCountInternal()const;

    Color GetElementalColorInternal(ElementalEffect type)const;

    void ResetInternal();

public:
    const int MAX_COMBO_LENGTH = 3;

    static float GetSpawnInterval() { return Get().m_SpawnInterval; }

    static void SetSpawnInterval(float interval) { Get().SetSpawnIntervalInternal(interval); }

    static void DecreaseSpawnInterval(float factor) { Get().DecreaseSpawnIntervalInternal(factor); }

    static void AddSpecialBubble(BubbleType type) { Get().AddSpecialBubbleInternal(type); }

    // static bool ElectroShieldActive() { return Get().m_ElectroShieldActive; }

    static ElementalEffect GetActiveElementalEffect() { return Get().m_ActiveElementalEffect; }


    // static void Update(float dT) override { Get()->UpdateInternal(dT); }

    static Color GetElementalColor(ElementalEffect type) { return Get().GetElementalColorInternal(type); }

    static int GetComboCountForType(ElementalEffect type) { return Get().m_ComboCount[(int)type];}

    static void Reset() { Get().ResetInternal(); }

    void Start() override;

    void Draw() const override { DrawComboCountInternal();}

    void Update(float dT) override;
};

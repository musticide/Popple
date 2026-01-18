#pragma once

#include "Entity.h"
#include "Log.h"
#include "SceneManager.h"
#include "Signal.h"
#include "Singleton.h"
#include "StaticMesh.h"
#include <algorithm>
#include <array>
#include <memory>
#include <raymob.h>
#include <raymath.h>

enum class ElementType { NONE, ELECTRO, ANEMO, COUNT };

class GameManager : public Entity, public Singleton<GameManager> {
public:
    GameManager();
    ~GameManager();


    Signal<ElementType> activeElementEffectChanged;
    ElementType GetActiveEffect() const { return m_ActiveEffect; }
    void AddSpecialBubbleInternal(ElementType type);

    /// 1st arg current score
    /// 2nd arg score delta
    Signal<int, int> scoreChanged;
    int GetScore() const { return m_Score; }
    void AddScore(int points);

    /// 1st arg current health
    /// 2nd arg health delta
    Signal<int, int> healthChanged;
    int GetHealth() const { return m_Health;}
    void DecreaseHealth(int amount);

    /// 1st arg current spawn Interval
    /// 2nd arg spawnInterval delta
    Signal<float, float> spawnIntervalChanged;
    /**
     * @ Use During initialisation only.
     * @ Connect to spawnIntervalChanged for repeated calls
    */
    float GetSpawnInterval() const{return m_SpawnInterval;}
    void DecreaseSpawnInterval(float amount);

    void Start() override;
    void OnEnable() override;

    void Update(float dT = 1.0f) override;

    int GetComboCountForType(ElementType type) { return m_ComboCount[(int)type]; }
    const int GetMaxComboLength() const { return MAX_COMBO_LENGTH; }

    const float electroShieldRadius = 10.f;


private:
    const int MAX_COMBO_LENGTH = 3;

    ElementType m_ActiveEffect = ElementType::NONE;
    int m_Score = 0;
    int m_Health = 100;
    std::array<int, (int)ElementType::COUNT> m_ComboCount;

    float m_SpawnInterval = 1.5f;
    float m_MinSpawnInterval = 0.20f;

    float m_ElectroShieldTimer = 0.0f;
    const float ELECTRO_SHIELD_DURATION = 5.0f;

    float m_AnemoEffectTimer = 0.0f;
    const float ANEMO_EFFECT_DURATION = 1.0f;

    void ResetComboCount();

    void EndGame();

    void ResetGameValues();

    void SetElectroShield(bool active);
};

#pragma once

#include "Entity.h"
#include "LevelConfig.h"
#include "Signal.h"
#include "Singleton.h"
#include "spatialGrid.h"
#include <array>
#include <memory>
#include <raymob.h>
#include <raymath.h>

class BubbleManager;
class EffectManager;

enum class ElementType {
    ELECTRO,
    ANEMO,
    CRYO,
    NONE,
    COUNT
};
static struct GameData {
    static constexpr float electroShieldRadius     = 10.f;
    static constexpr float ELECTRO_SHIELD_DURATION = 5.0f;
    static constexpr float CRYO_SHIELD_DURATION    = 4.0f;
    static constexpr float ANEMO_EFFECT_DURATION   = 1.0f;
    static constexpr int MAX_COMBO_LENGTH          = 3;
    static int availableElementCount;
} gameData;

class GameManager : public Entity, public Singleton<GameManager> {
  public:
    GameManager(Scene* parentScene, LevelConfig config);
    ~GameManager();

    void AddSpecialBubbleInternal(ElementType type);

    /// 1st arg current score
    /// 2nd arg score delta
    Signal<int, int> scoreChanged;
    int GetScore() const {
        return m_Score;
    }
    void AddScore(int points);

    /// 1st arg current health
    /// 2nd arg health delta
    Signal<int, int> healthChanged;
    int GetHealth() const {
        return m_Health;
    }
    void DecreaseHealth(int amount);

    void Start() override;
    void OnEnable() override;

    void Update(float dT = 1.0f) override;

    int GetComboCountForType(ElementType type) {
        return m_ComboCount[(int)type];
    }
    // const int GetMaxComboLength() const { return GameData::MAX_COMBO_LENGTH; }
    void ResetComboCount(ElementType type);

    static void PauseBubbleSpawn(bool pause);

    LevelConfig levelConfig;

    std::unique_ptr<EffectManager> effectManager = nullptr;
    std::unique_ptr<BubbleManager> bubbleManager = nullptr;
    std::unique_ptr<SpatialGrid> spatialGrid = nullptr;

  private:
    LevelConfig m_LevelConfig;

    int m_Score  = 0;
    int m_Health = 100;
    std::array<int, (int)ElementType::COUNT> m_ComboCount;

    float gameStartTime      = 0.0f;

    void ResetComboCount();

    void EndGame();

    void ResetGameValues();

    void SetElectroShield(bool active);
};

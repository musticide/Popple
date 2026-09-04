#pragma once

#include "ElementType.h"
#include "EndGameCanvas.h"
#include "Entity.h"
#include "LevelConfig.h"
#include "PauseGameCanvas.h"
#include "Signal.h"
#include "Singleton.h"
#include "spatialGrid.h"
#include "uiCanvas.h"
#include <array>
#include <memory>
#include <raymath.h>
#include <raymob.h>

class GameCanvas;

class BubbleManager;
class EffectManager;

static struct GameData {
    static constexpr float electroShieldRadius     = 10.f;
    static constexpr float ELECTRO_SHIELD_DURATION = 5.0f;
    static constexpr float CRYO_SHIELD_DURATION    = 4.0f;
    static constexpr float ANEMO_EFFECT_DURATION   = 1.0f;
    static constexpr int MAX_COMBO_LENGTH          = 3;
    static constexpr int MAX_SCORE                 = 500;
    static constexpr int BUBBLE_POINTS             = 10;
    static constexpr int BUBBLE_DAMAGE             = 20;
    static int availableElementCount;
} gameData;

class GameManager : public Entity, public Singleton<GameManager> {
  public:
    GameManager(Scene* parentScene, LevelParams config, GameCanvas* gameCanvas, PauseGameCanvas* pauseCanvas, EndGameCanvas* endGameCanvas);

    ~GameManager();

    void AddSpecialBubbleInternal(ElementType type);

    /// 1st arg current score
    /// 2nd arg score delta
    Signal<int, int> scoreChanged;
    int GetScore() const {
        return m_Score;
    }
    void AddScore();

    /// 1st arg current health
    /// 2nd arg health delta
    Signal<int, int> healthChanged;
    int GetHealth() const {
        return m_Health;
    }
    void DecreaseHealth();

    void Start() override;
    void OnEnable() override;

    void Update(float dT = 1.0f) override;

    int GetComboCountForType(ElementType type) {
        return m_ComboCount[(int)type];
    }
    // const int GetMaxComboLength() const { return GameData::MAX_COMBO_LENGTH; }
    void ResetComboCount(ElementType type);

    static void PauseBubbleSpawn(bool pause);

    LevelParams levelParams;

    std::unique_ptr<EffectManager> effectManager = nullptr;
    std::unique_ptr<BubbleManager> bubbleManager = nullptr;
    std::unique_ptr<SpatialGrid> spatialGrid     = nullptr;

    GameCanvas* gameCanvas;
    EndGameCanvas* endGameCanvas;
    PauseGameCanvas* pauseGameCanvas;

    void RestartGame(LevelParams params);

  private:
    int m_Score  = 0;
    int m_Health = 100;
    std::array<int, (int)ElementType::COUNT> m_ComboCount;

    float gameStartTime = 0.0f;

    void ResetComboCount();

    void PauseGame();
    void ResumeGame();
    void EndGame();

    void ResetGameValues();

    void StopGameSystems();
    void StartGameSystems();

    void SetElectroShield(bool active);
};

#include "GameManager.h"
#include "EffectManager.h"
#include "LevelConfig.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "Scene.h"
#include "SceneManager.h"
#include "bubbleManager.h"
#include "raylib.h"

int GameData::availableElementCount = 3;

GameManager::GameManager(Scene* parentScene, LevelConfig config, ui::Canvas* gameCanvas, ui::Canvas* pauseCanvas, ui::Canvas* endGameCanvas)
: Entity(parentScene)
, gameCanvas(gameCanvas)
, pauseGameCanvas(pauseCanvas)
, endGameCanvas(endGameCanvas)
, levelConfig(config) {
    effectManager = parentScene->CreateEntity<EffectManager>(true);
    spatialGrid   = parentScene->CreateEntity<SpatialGrid>(true);
    bubbleManager = parentScene->CreateEntity<BubbleManager>(true, levelConfig);
}
GameManager::~GameManager() {
    LOGI("Game Man Destroyed");
}

void GameManager::AddSpecialBubbleInternal(ElementType type) {
    if (type != ElementType::NONE) {
        for (int i = 0; i < (int)ElementType::COUNT - 1; i++) {
            if (i == (int)type && !EffectManager::Get().IsEffectCharged(type)) {
                m_ComboCount[i]++;
            } else
                m_ComboCount[i] = 0;

            LOGI("Combo Count %d = %d", i, m_ComboCount[i]);
        }

        if (m_ComboCount[(int)type] >= GameData::MAX_COMBO_LENGTH) {
            m_ComboCount[(int)type] = 0;
            EffectManager::Get().ChargeEffect(type);
        }
    }
}

void GameManager::AddScore(int points) {
    m_Score += points;
    scoreChanged(m_Score, points);
    if (m_Score >= GameData::MAX_SCORE) {
        // GAME WON
        EndGame(true);
    }
}

void GameManager::DecreaseHealth(int amount) {
    m_Health -= amount;
    m_Health = std::max(m_Health, 0);
    healthChanged(m_Health, amount);
    if (m_Health <= 0) {
        EndGame(false);
    }
}

void GameManager::Start() {
    ResetGameValues();
    // activeElementEffectChanged(activeEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    LOGI("Game Manager Start");
}

void GameManager::OnEnable() {
    LOGI("Game Manager Enabled");
    StartGameSystems();
}

void GameManager::Update(float dT) {
}

void GameManager::ResetComboCount() {
    std::fill(m_ComboCount.begin(), m_ComboCount.end(), 0);
}

void GameManager::ResetComboCount(ElementType type) {
    m_ComboCount[(int)type] = 0;
}

void GameManager::EndGame(bool hasWon) {

    StopGameSystems();
    gameCanvas->SetActive(false);
    endGameCanvas->SetActive(true);

    if (hasWon) {

        if (levelConfig.level > PlayerProfile.highestLevelCleared.value)
            PlayerProfile.highestLevelCleared.value = levelConfig.level;
    }

    if (PlayerProfile.highestScore.value < m_Score) {
        PlayerProfile.highestScore.value = m_Score;
    }
    float highestTime = GetTime() - gameStartTime;
    if (PlayerProfile.longestTimeSurvived.value < highestTime) {
        PlayerProfile.longestTimeSurvived.value = highestTime;
    }

    SavePlayerProfile();
    ResetGameValues();
}


void GameManager::ResetGameValues() {
    m_Score  = 0;
    m_Health = 100;
    ResetComboCount();
    gameStartTime = 0.0f;

    EffectManager::Get().Reset();
    BubbleManager::Get().Reset();
}
void GameManager::PauseBubbleSpawn(bool pause) {
    if (pause) {
        BubbleManager::Get().PauseSpawn();
    } else {
        BubbleManager::Get().ContinueSpawn();
    }
}
void GameManager::StopGameSystems() {
    effectManager->SetActive(false);
    bubbleManager->SetActive(false);
    spatialGrid->SetActive(false);
}
void GameManager::StartGameSystems() {
    ResetGameValues();
    // activeElementEffectChanged(activeEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    gameStartTime = GetTime();

    effectManager->SetActive(true);
    bubbleManager->SetActive(true);
    spatialGrid->SetActive(true);
}


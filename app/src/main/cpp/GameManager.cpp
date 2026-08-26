#include "GameManager.h"
#include "EffectManager.h"
#include "Globals.h"
#include "LevelConfig.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "Scene.h"
#include "SceneManager.h"
#include "bubbleManager.h"
#include "raylib.h"
#include <cmath>

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

void GameManager::AddScore() {
    m_Score += GameData::BUBBLE_POINTS;
    scoreChanged(m_Score, GameData::BUBBLE_POINTS);
    if (m_Score >= GameData::MAX_SCORE) {
        // GAME WON
        EndGame();
    }
}

void GameManager::DecreaseHealth() {
    m_Health -= GameData::BUBBLE_DAMAGE;
    m_Health = std::max(m_Health, 0);
    healthChanged(m_Health, GameData::BUBBLE_DAMAGE);
    if (m_Health <= 0) {
        EndGame();
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

void GameManager::EndGame() {
    using namespace Globals;

    StopGameSystems();

    GameResults.levelPlayed   = levelConfig.levelNumber;
    GameResults.gameCompleted = true;

    if (m_Score > levelConfig.minScore) {
        GameResults.gameWon = true;

        if (levelConfig.levelNumber > PlayerProfile.highestLevelCleared.value) {
            PlayerProfile.highestLevelCleared.value = levelConfig.levelNumber;
            LOGI("PLAYER_PROFILE: Highest level cleared %d", PlayerProfile.highestLevelCleared.value);
        }
    } else {
        GameResults.gameWon = false;
    }
    GameResults.score  = m_Score;
    GameResults.health = m_Health;

    if (PlayerProfile.highestScore.value < m_Score) {
        PlayerProfile.highestScore.value = m_Score;
    }
    float highestTime = GetTime() - gameStartTime;
    if (PlayerProfile.longestTimeSurvived.value < highestTime) {
        PlayerProfile.longestTimeSurvived.value = highestTime;
    }

    GameResults.levelRating = static_cast<int>(std::floor((float)m_Score / levelConfig.minScore));
    PlayerProfile.levelsData.value[levelConfig.levelNumber - 1] = {
        .rating = GameResults.levelRating, .score = m_Score, .time = static_cast<int>(highestTime)
    };

    SyncLevelToFirebase(levelConfig.levelNumber);
    ResetGameValues();

    gameCanvas->SetActive(false);
    endGameCanvas->SetActive(true);
}


void GameManager::ResetGameValues() {
    m_Score  = 0;
    m_Health = 100;
    ResetComboCount();
    gameStartTime = GetTime();

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
    bubbleManager->levelConfig = levelConfig;
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    gameStartTime = GetTime();

    effectManager->SetActive(true);
    bubbleManager->SetActive(true);
    spatialGrid->SetActive(true);
}
void GameManager::RestartGame(LevelConfig config) {
    LOGI("Game Restarted");
    levelConfig = config;
    StartGameSystems();
    gameCanvas->SetActive(true);
    endGameCanvas->SetActive(false);
}
void GameManager::PauseGame() {
    Globals::gamePaused = true;
}

void GameManager::ResumeGame() {
    Globals::gamePaused = false;
}

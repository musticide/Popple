#include "GameManager.h"
#include "EffectManager.h"
#include "Game.h"
#include "Globals.h"
#include "Log.h"
#include "Scene.h"
#include "SceneManager.h"
#include "bubbleManager.h"
#include "raylib.h"
#include "raymob.h"

int GameData::availableElementCount = 3;

GameManager::GameManager() {
}
GameManager::~GameManager() {
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
}

void GameManager::DecreaseHealth(int amount) {
    m_Health -= amount;
    m_Health = std::max(m_Health, 0);
    healthChanged(m_Health, amount);
    if (m_Health <= 0) {
        EndGame();
    }
}

void GameManager::DecreaseSpawnInterval(float amount) {
    m_SpawnInterval -= amount;
    m_SpawnInterval = std::max(m_SpawnInterval, m_MinSpawnInterval);
    spawnIntervalChanged(m_SpawnInterval, amount);
}

void GameManager::Start() {
    ResetGameValues();
    // activeElementEffectChanged(activeEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    spawnIntervalChanged(m_SpawnInterval, 0.f);
    LOGI("Game Manager Start");
}

void GameManager::OnEnable() {
    LOGI("Game Manager Enabled");
    ResetGameValues();
    // activeElementEffectChanged(activeEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    spawnIntervalChanged(m_SpawnInterval, 0.f);
    gameStartTime = GetTime();
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
    SceneManager::Get().ActivateScene(SceneType::HOME);
    SceneManager::Get().DeactivateScene(SceneType::GAMEPLAY);

    if (Globals::highScore < m_Score) {
        Globals::highScore = m_Score;
        WriteToAppStorage("highscore.dat", &m_Score, sizeof(int));
    }
    float highestTime = GetTime() - gameStartTime;
    if(Globals::highestTime < highestTime)
    {
        Globals::highestTime = highestTime;
        WriteToAppStorage("highest_time.dat", &highestTime, sizeof(float));
    }
}

void GameManager::ResetGameValues() {
    m_Score  = 0;
    m_Health = 100;
    ResetComboCount();
    gameStartTime = 0.0f;

    m_SpawnInterval    = 1.5f;
    m_MinSpawnInterval = 0.20f;
    EffectManager::Get().Reset();
}
void GameManager::PauseBubbleSpawn(bool pause) {
    if (pause) {
        BubbleManager::Get().PauseSpawn();
    } else {
        BubbleManager::Get().ContinueSpawn();
    }
}

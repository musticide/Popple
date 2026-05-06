#include "GameManager.h"
#include "EffectManager.h"
#include "Log.h"
#include "Scene.h"
#include "SceneManager.h"
#include "bubbleManager.h"

GameManager::GameManager() {
}
GameManager::~GameManager() {
}

void GameManager::AddSpecialBubbleInternal(ElementType type) {
    if (type != ElementType::NONE) {
        for (int i = 1; i < (int)ElementType::COUNT; i++) {
            if (i == (int)type && !EffectManager::Get().IsEffectCharged(type)) {
                m_ComboCount[i]++;
            } else
                m_ComboCount[i] = 0;

            LOGI("Combo Count %d = %d", i, m_ComboCount[i]);
        }

        switch (type) {
            case ElementType::ELECTRO:
                if (m_ComboCount[(int)ElementType::ELECTRO] >= GameData::MAX_COMBO_LENGTH) {
                    // LOGI("Electro Shield Activated");
                    // activeEffect                            = ElementType::ELECTRO;
                    // activeElementEffectChanged(ElementType::ELECTRO);
                    // EffectManager::Get().ActivateElectroShield();
                    // NOTE:^^^ MOVED TO Effect Manager
                    m_ComboCount[(int)ElementType::ELECTRO] = 0;
                    EffectManager::Get().ChargeEffect(ElementType::ELECTRO);
                }
                break;
            case ElementType::ANEMO:
                if (m_ComboCount[(int)ElementType::ANEMO] >= GameData::MAX_COMBO_LENGTH) {
                    // LOGI("Anemo Activated");
                    // activeEffect                          = ElementType::ANEMO;
                    // activeElementEffectChanged(ElementType::ANEMO);
                    // EffectManager::Get().ActivateAnemoShield();
                    // NOTE:^^^ MOVED TO Effect Manager
                    m_ComboCount[(int)ElementType::ANEMO] = 0;
                    EffectManager::Get().ChargeEffect(ElementType::ANEMO);
                }
                break;
            default:
                LOGE("Bubble Type Out of range");
                break;
        }
    }
}

void GameManager::AddScore(int points) {
    m_Score += points;
    scoreChanged(m_Score, points);
    if (m_Score >= 1000) {
        // game won
        EndGame();
    }
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
}

void GameManager::ResetGameValues() {
    m_Score  = 0;
    m_Health = 100;
    ResetComboCount();

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

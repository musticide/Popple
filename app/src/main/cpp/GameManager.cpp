#include "GameManager.h"
#include "Log.h"
#include "bubbleManager.h"

GameManager::GameManager() { }
GameManager::~GameManager() { }

void GameManager::AddSpecialBubbleInternal(ElementType type)
{
    if (type != ElementType::NONE) {
        for (int i = 1; i < (int)ElementType::COUNT; i++) {
            if (i == (int)type)
                m_ComboCount[i]++;
            else
                m_ComboCount[i] = 0;

            LOGI("Combo Count %d = %d", i, m_ComboCount[i]);
        }

        switch (type) {
        case ElementType::ELECTRO:
            if (m_ComboCount[(int)ElementType::ELECTRO] >= MAX_COMBO_LENGTH) {
                LOGI("Electro Shield Activated");
                m_ComboCount[(int)ElementType::ELECTRO] = 0;
                m_ActiveEffect = ElementType::ELECTRO;
                activeElementEffectChanged(ElementType::ELECTRO);
            }
            break;
        case ElementType::ANEMO:
            if (m_ComboCount[(int)ElementType::ANEMO] >= MAX_COMBO_LENGTH) {
                LOGI("Anemo Activated");
                m_ComboCount[(int)ElementType::ANEMO] = 0;
                m_ActiveEffect = ElementType::ANEMO;
                activeElementEffectChanged(ElementType::ANEMO);
            }
            break;
        default:
            LOGE("Bubble Type Out of range");
            break;
        }
    }
}

void GameManager::AddScore(int points)
{
    m_Score += points;
    scoreChanged(m_Score, points);
    if (m_Score >= 1000) {
        // game won
        EndGame();
    }
}

void GameManager::DecreaseHealth(int amount)
{
    m_Health -= amount;
    m_Health = std::max(m_Health, 0);
    healthChanged(m_Health, amount);
    if (m_Health <= 0) {
        EndGame();
    }
}

void GameManager::DecreaseSpawnInterval(float amount)
{
    m_SpawnInterval -= amount;
    m_SpawnInterval = std::max(m_SpawnInterval, m_MinSpawnInterval);
    spawnIntervalChanged(m_SpawnInterval, amount);
}

void GameManager::Start()
{
    LOGI("Game Manager Start");
    ResetGameValues();
    activeElementEffectChanged(m_ActiveEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    spawnIntervalChanged(m_SpawnInterval, 0.f);
}

void GameManager::OnEnable()
{
    LOGI("Game Manager Enabled");
    ResetGameValues();
    activeElementEffectChanged(m_ActiveEffect);
    scoreChanged(m_Score, 0);
    healthChanged(m_Health, 0);
    spawnIntervalChanged(m_SpawnInterval, 0.f);
}

void GameManager::Update(float dT)
{
    switch (m_ActiveEffect) {
    case ElementType::NONE:
        break;
    case ElementType::ELECTRO:
        m_ElectroShieldTimer += dT;
        if (m_ElectroShieldTimer > ELECTRO_SHIELD_DURATION) {
            m_ActiveEffect = ElementType::NONE;
            activeElementEffectChanged(m_ActiveEffect);
            m_ElectroShieldTimer = 0.0f;
        }
        break;
    case ElementType::ANEMO:
        m_AnemoEffectTimer += dT;
        BubbleManager::Get().PauseSpawn();
        if (m_AnemoEffectTimer > ANEMO_EFFECT_DURATION) {
            m_ActiveEffect = ElementType::NONE;
            activeElementEffectChanged(m_ActiveEffect);
            BubbleManager::Get().ContinueSpawn();
            m_AnemoEffectTimer = 0.0f;
        }
        break;
    default:
        LOGE("Active effect index out of bounds");
        break;
    }
}

void GameManager::ResetComboCount() { std::fill(m_ComboCount.begin(), m_ComboCount.end(), 0); }

void GameManager::EndGame()
{
    SceneManager::Get().ActivateScene("HomeScene");
    SceneManager::Get().DeactivateScene("GameplayScene");
}

void GameManager::ResetGameValues()
{
    m_ActiveEffect = ElementType::NONE;
    m_Score = 0;
    m_Health = 100;
    ResetComboCount();

    m_SpawnInterval = 1.5f;
    m_MinSpawnInterval = 0.20f;

    m_ElectroShieldTimer = 0.0f;

    m_AnemoEffectTimer = 0.0f;
}

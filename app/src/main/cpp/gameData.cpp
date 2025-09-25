#include "gameData.h"
#include "Log.h"
#include "bubble.h"

GameData::GameData() { }
GameData::~GameData() { }

void GameData::DecreaseSpawnIntervalInternal(float factor)
{
    if (m_SpawnInterval > m_MinSpawnInterval)
        m_SpawnInterval -= factor;
    LOGI("Spawn Interval Decreased to %f", m_SpawnInterval);
}

void GameData::SetSpawnIntervalInternal(float interval) { m_SpawnInterval = interval; }
void GameData::UpdateInternal(float dT)
{
    if (m_ElectroShieldActive) {
        m_ElectroShieldTimer += dT;
        if (m_ElectroShieldTimer >= m_ShieldDuration) {
            m_ElectroShieldActive = false;
            m_ElectroShieldTimer = 0.0f;
        }
    }
}

void GameData::AddSpecialBubbleInternal(BubbleType type)
{
    LOGI("Anemo Combo Count: %d", m_ComboCount[ANEMO_BUBBLE]);
    m_ComboCount[type]++;
    switch (type) {
    case DEFAULT_BUBBLE:
        break;
    case ELECTRO_BUBBLE:
        if (m_ComboCount[ELECTRO_BUBBLE] >= 3) {
            m_ComboCount[ELECTRO_BUBBLE] = 0;
            m_ElectroShieldActive = true;
        }
        break;
    case ANEMO_BUBBLE:
        if (m_ComboCount[ANEMO_BUBBLE] >= 3) {
            m_ComboCount[ANEMO_BUBBLE] = 0;
            // m_AnemoBlast = true;
            ActivateAnemoBlast();
        }
        break;
    }
}

void GameData::ActivateAnemoBlast()
{
    LOGI("Anemo Blast Not implemented");
   //add force to all bubbles
}

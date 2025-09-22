#include "gameParams.h"
#include "Log.h"

GameParams::GameParams() { }
GameParams::~GameParams() { }

void GameParams::DecreaseSpawnIntervalInternal(float factor)
{
    if (m_SpawnInterval > m_MinSpawnInterval)
        m_SpawnInterval -= factor;
    LOGI("Spawn Interval Decreased to %f", m_SpawnInterval);
}

void GameParams::SetSpawnIntervalInternal(float interval) { m_SpawnInterval = interval; }

#include "bubbleManager.h"
#include "Log.h"

BubbleManager::BubbleManager() { LOGI("Bubble Manager Initialized"); }

BubbleManager::~BubbleManager() { }

void BubbleManager::Update(float dT) { SpawnBubbleInternal(); }

void BubbleManager::SpawnBubbleInternal()
{
    // LOGI("Spawn Bubble called");
    m_SpawnTimer += GetFrameTime();
    float spawnInterval = GameData::GetSpawnInterval();

    if (m_SpawnTimer > spawnInterval) {
        m_SpawnTimer = 0.0f;
        for (int i = 0; i < m_MaxBubbleCount; i++) {
            if (!m_Bubbles[i].IsActive()) {
                // LOGI("Bubble Spawned at frame: %f", GetTime());
                m_Bubbles[i].SetActive(true);
                m_Bubbles[i].Spawn();
                break;
            }
        }
    }
}

void BubbleManager::Init() { Get(); }
void BubbleManager::Start() { SetActive(true); }

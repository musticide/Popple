#include "bubbleManager.h"
#include "Log.h"
#include "gameData.h"
#include "raymath.h"
#include "score.h"

BubbleManager::BubbleManager() { LOGI("Bubble Manager Initialized"); }

BubbleManager::~BubbleManager() { }

void BubbleManager::Update(float dT)
{
    SpawnBubbleInternal();

    for (int i = 0; i < m_MaxBubbleCount; i++) {
        /* if(m_Bubbles[i] == nullptr){
            LOGE("Null Reference: Bubble Missing");
            continue;
        } */

        if (m_Bubbles[i].IsActive()) {

            for (int j = 0; j < GetTouchPointCount(); j++) {
                if (m_Bubbles[i].IsPointInBubble(Input::Get().GetTouchPositionWS(j))) {
                    Score::AddScore(5);
                    GameData::DecreaseSpawnInterval(0.01f);
                    GameData::AddSpecialBubble(m_Bubbles[i].GetType());
                    m_Bubbles[i].SetActive(false);
                    continue;
                }
            }

            switch (GameData::GetActiveElementalEffect()) {
            case NO_ELEMENTAL_EFFECT:
                break;

            case ELECTRO:
                if (Vector2Length(m_Bubbles[i].position) <= m_Bubbles[i].radius + 200) {
                    m_Bubbles[i].SetActive(false);
                }
                break;

            case ANEMO:
            default:
                break;
            }

            if (m_Bubbles[i].IsActive()) {
                if (Vector2Length(m_Bubbles[i].position) <= m_Bubbles[i].radius + 50) {
                    Score::DecreaseHealth(10);
                    m_Bubbles[i].SetActive(false);
                    continue;
                }
            }
        }
    }
}

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

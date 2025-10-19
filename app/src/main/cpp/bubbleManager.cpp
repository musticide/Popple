#include "bubbleManager.h"
#include "Log.h"
#include "bubble.h"
#include "gameData.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"

BubbleManager::BubbleManager() { LOGI("Bubble Manager Initialized"); }

BubbleManager::~BubbleManager() { delete[] m_Bubbles; }

void BubbleManager::DoAnemoBlast(Bubble& bubble)
{
    LOGI("Anemo Blast Force applied");
    Vector3 force = Vector3Normalize(bubble.position);
    force = Vector3Scale(force, 500);
    bubble.AddForce(force);
}
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
                // Vector3 touchPos = Input::GetTouchPositionWS(j);
                // LOGI("Touch Pos: %f, %f, %f", touchPos.x, touchPos.y, touchPos.z);
                if (m_Bubbles[i].IsPointInBubble(Input::GetTouchPositionWS(j))) {
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
                if (Vector3Length(m_Bubbles[i].position) <= m_Bubbles[i].radius + 4) {
                    m_Bubbles[i].SetActive(false);
                }
                break;

            case ANEMO:
                DoAnemoBlast(m_Bubbles[i]);
                //     Vector3 force = m
                // m_Bubbles[i].AddForce(Vector3 force)
            default:
                break;
            }

            if (m_Bubbles[i].IsActive()) {
                if (Vector3Length(m_Bubbles[i].position) <= m_Bubbles[i].radius + 2) {
                    if (m_Bubbles[i].GetType() == DEFAULT_BUBBLE) {
                        Score::DecreaseHealth(10);
                    }
                    m_Bubbles[i].SetActive(false);
                    LOGI("Bubble Reset");
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
void BubbleManager::Start() {}

void BubbleManager::ResetInternal()
{

    for (int i = 0; i < m_MaxBubbleCount; i++) {
        m_Bubbles[i].SetActive(false);
    }
}

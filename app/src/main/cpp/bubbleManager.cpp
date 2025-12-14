#include "bubbleManager.h"
#include "GameManager.h"
#include "Log.h"
#include "Scene.h"
#include "bubble.h"
#include "input.h"
#include "raymath.h"
#include "spatialGrid.h"
#include <raymob.h>

BubbleManager::BubbleManager() { LOGI("Bubble Manager constructed"); }

BubbleManager::~BubbleManager() { }

ElementType BubbleManager::s_ActiveEffect;
float BubbleManager::s_SpawnInterval;

void BubbleManager::DoAnemoBlast(Bubble* bubble)
{
    // LOGI("Anemo Blast Force applied");
    Vector3 force = Vector3Normalize(bubble->position);
    force = Vector3Scale(force, 500);
    bubble->AddForce(force);
}
void BubbleManager::Update(float dT)
{
    SpatialGrid::Clear();
    SpawnBubbleInternal();

    for (int i = 0; i < m_MaxBubbleCount; i++) {
        if (m_Bubbles[i]->IsActive()) {

            for (int j = 0; j < GetTouchPointCount(); j++) {
                // LOGI("Touch Pos: %f, %f, %f", touchPos.x, touchPos.y, touchPos.z);
                if (m_Bubbles[i]->IsPointInBubble(Input::GetTouchPositionWS(j))) {
                    GameManager::Get().AddScore(5);
                    GameManager::Get().DecreaseSpawnInterval(0.01f);
                    GameManager::Get().AddSpecialBubbleInternal(m_Bubbles[i]->GetType());
                    m_Bubbles[i]->SetActive(false);
                    continue;
                }
            }

            switch (s_ActiveEffect) {
            case ElementType::NONE:
                break;

            case ElementType::ELECTRO:
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + 4) {
                    m_Bubbles[i]->SetActive(false);
                }
                break;

            case ElementType::ANEMO:
                DoAnemoBlast(m_Bubbles[i].get());
            default:
                break;
            }

            if (m_Bubbles[i]->IsActive()) {
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + 2) {
                    if (m_Bubbles[i]->GetType() == ElementType::NONE) {
                        GameManager::Get().DecreaseHealth(10);
                    }
                    m_Bubbles[i]->SetActive(false);
                    LOGI("Bubble Reset");
                    continue;
                }
            }
        }
    }
}

void BubbleManager::SpawnBubbleInternal()
{
    if (m_PauseSpawn)
        return;
    m_SpawnTimer += GetFrameTime();

    if (m_SpawnTimer > s_SpawnInterval) {
        m_SpawnTimer = 0.0f;
        for (int i = 0; i < m_MaxBubbleCount; i++) {
            if (!m_Bubbles[i]->IsActive()) {
                // LOGI("Bubble Spawned at frame: %f", GetTime());
                m_Bubbles[i]->SetActive(true);
                m_Bubbles[i]->Spawn();
                break;
            }
        }
    }
}

void BubbleManager::Start()
{
    LOGV("Bubble start");
    for (int i = 0; i < m_MaxBubbleCount; i++) {
        if (parentScene != nullptr) {
            m_Bubbles.push_back(std::move(parentScene->CreateEntity<Bubble>(false)));
        }
    }
    LOGV("Bubbles generated");

    s_ActiveEffect = GameManager::Get().GetActiveEffect();
    s_SpawnInterval = GameManager::Get().GetSpawnInterval();

    GameManager::Get().activeElementEffectChanged.connect(BubbleManager::ActiveEffectChanged);
    GameManager::Get().spawnIntervalChanged.connect(BubbleManager::SpawnIntervalChanged);
}

void BubbleManager::OnEnable()
{
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i]->SetActive(false);
    }
}

void BubbleManager::ResetInternal()
{
    for (int i = 0; i < m_MaxBubbleCount; i++) {
        m_Bubbles[i]->SetActive(false);
    }
}
void BubbleManager::ActiveEffectChanged(ElementType type) { s_ActiveEffect = type; }
void BubbleManager::SpawnIntervalChanged(float spawnInterval, float amount) { s_SpawnInterval = spawnInterval; }
void BubbleManager::ContinueSpawn() { m_PauseSpawn = false; }

void BubbleManager::PauseSpawn() { m_PauseSpawn = true; }

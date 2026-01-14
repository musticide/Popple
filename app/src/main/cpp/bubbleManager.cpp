#include "bubbleManager.h"
#include "GameManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "functionLibrary.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "spatialGrid.h"
#include <cstddef>
#include <memory>
#include <raymob.h>
#include <sys/types.h>

ElementType BubbleManager::s_ActiveEffect;
float BubbleManager::s_SpawnInterval;

BubbleManager::BubbleManager()
    : DrawableEntity(RenderQueue::TRANSPARENT)
{
    LOGI("Bubble Manager constructed");
}

BubbleManager::~BubbleManager() { }

void BubbleManager::Start()
{
    m_Bubbles.resize(INITIAL_POOL_SIZE);
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i] = std::make_unique<Bubble>();
    }

    s_ActiveEffect = GameManager::Get().GetActiveEffect();
    s_SpawnInterval = GameManager::Get().GetSpawnInterval();

    GameManager::Get().activeElementEffectChanged.connect(BubbleManager::ActiveEffectChanged);
    GameManager::Get().spawnIntervalChanged.connect(BubbleManager::SpawnIntervalChanged);

    m_BubbleBaseModel = ResourceManager::GetModel("models/BubbleBase_01.glb");

    bubbleMaterials[(int)ElementType::NONE] = LoadMaterialDefault();
    bubbleMaterials[(int)ElementType::NONE].shader
        = *ResourceManager::GetShader("shaders/CommonBubble_VS.glsl", "shaders/CommonBubble_PS.glsl");
    bubbleMaterials[(int)ElementType::NONE].maps[0].texture
        = *ResourceManager::GetTexture("textures/CommonBubble_UTIL.png");
    bubbleMaterials[(int)ElementType::NONE].maps[1].texture
        = *ResourceManager::GetTexture("textures/ColorGradient_01.png");
    commonTimeId = GetShaderLocation(bubbleMaterials[(int)ElementType::NONE].shader, "_Time");

    bubbleMaterials[(int)ElementType::ELECTRO] = LoadMaterialDefault();
    bubbleMaterials[(int)ElementType::ELECTRO].shader
        = *ResourceManager::GetShader("shaders/ElectroBubble_VS.glsl", "shaders/ElectroBubble_PS.glsl");
    bubbleMaterials[(int)ElementType::ELECTRO].maps[0].texture
        = *ResourceManager::GetTexture("textures/ElectroBubble_UTIL.png");
    electroTimeId = GetShaderLocation(bubbleMaterials[(int)ElementType::ELECTRO].shader, "_Time");

    bubbleMaterials[(int)ElementType::ANEMO] = LoadMaterialDefault();
    bubbleMaterials[(int)ElementType::ANEMO].shader
        = *ResourceManager::GetShader("shaders/AnemoBubble_VS.glsl", "shaders/AnemoBubble_PS.glsl");
    bubbleMaterials[(int)ElementType::ANEMO].maps[0].texture
        = *ResourceManager::GetTexture("textures/AnemoBubble_UTIL.png");
    anemoTimeId = GetShaderLocation(bubbleMaterials[(int)ElementType::ANEMO].shader, "_Time");
}

void BubbleManager::OnEnable() { ResetInternal(); }

void BubbleManager::DoAnemoBlast(Bubble* bubble)
{
    // LOGI("Anemo Blast Force applied");
    Vector3 force = Vector3Normalize(bubble->position);
    force = Vector3Scale(force, 500);
    // bubble->AddForce(force);
    bubble->velocity += force;
}

void BubbleManager::Update(float dT)
{
    SpatialGrid::Clear();
    SpawnBubbles();

    uint activeBubbles = 0;
    // Process Bubbles
    for (int i = 0; i < m_Bubbles.size(); i++) {
        if (m_Bubbles[i] != nullptr && m_Bubbles[i]->isActive) {
            activeBubbles++;
            // Physics and Collisions
            UpdateBubble(m_Bubbles[i].get());

            // Check if Bubble was tapped
            for (int j = 0; j < GetTouchPointCount(); j++) {
                // LOGI("Touch Pos: %f, %f, %f", touchPos.x, touchPos.y, touchPos.z);
                if (IsPointInBubble(m_Bubbles[i].get(), Input::GetTouchPositionWS(j))) {
                    GameManager::Get().AddScore(5);
                    GameManager::Get().DecreaseSpawnInterval(0.01f);
                    GameManager::Get().AddSpecialBubbleInternal(m_Bubbles[i]->type);
                    m_Bubbles[i]->isActive = false;
                    continue;
                }
            }

            switch (s_ActiveEffect) {
            case ElementType::NONE:
                break;
            case ElementType::ELECTRO:
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + 4) {
                    m_Bubbles[i]->isActive = false;
                }
                break;
            case ElementType::ANEMO:
                DoAnemoBlast(m_Bubbles[i].get());
                break;
            default:
                break;
            }

            // Check if bubble has reached center
            if (m_Bubbles[i]->isActive) {
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + 2) {
                    if (m_Bubbles[i]->type == ElementType::NONE) {
                        GameManager::Get().DecreaseHealth(10);
                    }
                    m_Bubbles[i]->isActive = false;
                    LOGI("Bubble Reset");
                    continue;
                }
            }
        }
    }
    LOGI("Active Bubble Count: %d", activeBubbles);
    float time = GetTime();
    SetShaderValue(bubbleMaterials[(int)ElementType::NONE].shader, commonTimeId, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(bubbleMaterials[(int)ElementType::ELECTRO].shader, electroTimeId, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(bubbleMaterials[(int)ElementType::ANEMO].shader, anemoTimeId, &time, SHADER_UNIFORM_FLOAT);
}

void BubbleManager::Draw() const
{
    for (size_t i = 0; i < m_Bubbles.size(); i++) {
        if (m_Bubbles[i] != nullptr && m_Bubbles[i]->isActive) {
            // Change Bubble Material based on bubble element type
            if ((int)m_Bubbles[i]->type < (int)ElementType::COUNT)
                m_BubbleBaseModel->materials[0] = bubbleMaterials[(int)m_Bubbles[i]->type];
            else {
                m_BubbleBaseModel->materials[0] = bubbleMaterials[(int)ElementType::NONE];
                LOGE("Bubble type out of bounds");
            }

            DrawModel(*m_BubbleBaseModel, m_Bubbles[i]->position, m_Bubbles[i]->radius, WHITE);
        }
    }
}

void BubbleManager::ActiveEffectChanged(ElementType type) { s_ActiveEffect = type; }

bool BubbleManager::IsPointInBubble(Bubble* bubble, Vector3 point) const
{
    return Vector3Length((Vector3) { point.x, 0, point.z } - bubble->position) <= bubble->radius;
}

/// returns position at a defined radius
Vector3 BubbleManager::GetRandomSpawnPos()
{
    // TODO: decrease distance
    float distance = GetRandomValue(40, 35);
    float randAngle = GetRandomValue(0, 360);
    return (Vector3) { (float)cos(randAngle) * distance, 0, (float)sin(randAngle) * distance };
}
void BubbleManager::SpawnBubble(Bubble* bubble)
{
    if (bubble == nullptr) {
        LOGW("Unable to spawn bubble : nullptr");
        return;
    }
    bubble->position = GetRandomSpawnPos();
    bubble->radius = GetRandomValue(20, 25) / 10.f;
    bubble->velocity = Vector3Scale(Vector3Normalize(Vector3Zero() - bubble->position), bubble->CENTER_FORCE);

    if (RollPercentage(40)) {
        bubble->type = RollPercentage(50) ? ElementType::ANEMO : ElementType::ELECTRO;
    } else {
        bubble->type = ElementType::NONE;
    }
}
/// Update Bubble posiiton and check for collisions with other bubbles
void BubbleManager::UpdateBubble(Bubble* bubble)
{
    bubble->velocity += Vector3Scale(Vector3Normalize(Vector3Zero() - bubble->position), bubble->CENTER_FORCE);
    bubble->ApplyForces();
    SpatialGrid::AddEntity(bubble);
    for (Bubble* nearby : SpatialGrid::GetNearbyEntities(bubble->position)) {
        if (CheckCollisionSpheres(bubble->position, bubble->radius, nearby->position, nearby->radius)) {
            bubble->ResolveCollision(nearby);
        }
    }
}
void BubbleManager::SpawnBubbles()
{
    if (m_PauseSpawn)
        return;
    m_SpawnTimer += GetFrameTime();

    if (m_SpawnTimer > s_SpawnInterval) {
        for (int i = 0; i < m_Bubbles.size(); i++) {
            if (!m_Bubbles[i]->isActive) {
                // LOGI("Bubble Spawned at frame: %f", GetTime());
                m_Bubbles[i]->isActive = true;
                // m_Bubbles[i]->Spawn();
                SpawnBubble(m_Bubbles[i].get());
                m_SpawnTimer = 0.0f;
                break;
            }
        }
        if (m_SpawnTimer > 0.0001f) {
            LOGE("Bubble Pool Exhaused");
        }
    }
}
void BubbleManager::PauseSpawn() { m_PauseSpawn = true; }
void BubbleManager::ContinueSpawn() { m_PauseSpawn = false; }
void BubbleManager::SpawnIntervalChanged(float spawnInterval, float amount) { s_SpawnInterval = spawnInterval; }

void BubbleManager::ResetInternal()
{
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i]->isActive = false;
    }
}

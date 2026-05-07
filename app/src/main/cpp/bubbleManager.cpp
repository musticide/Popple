#include "bubbleManager.h"
#include "EffectManager.h"
#include "Game.h"
#include "GameCanvas.h"
#include "GameManager.h"
#include "Log.h"
#include "ParticleSystem.h"
#include "ResourceManager.h"
#include "functionLibrary.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "spatialGrid.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <raymob.h>
#include <sys/types.h>

float BubbleManager::s_SpawnInterval;
float Bubble::speedMultiplier = 1.f;

BubbleManager::BubbleManager()
: DrawableEntity(RenderQueue::OPAQUE) {
    LOGI("Bubble Manager constructed");
}

BubbleManager::~BubbleManager() {
}

void BubbleManager::Start() {
    m_Bubbles.resize(INITIAL_POOL_SIZE);
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i] = std::make_unique<Bubble>();
    }

    s_SpawnInterval = GameManager::Get().GetSpawnInterval();

    GameManager::Get().spawnIntervalChanged.connect(BubbleManager::SpawnIntervalChanged);

    m_BubbleBaseModel = ResourceManager::GetModel("models/BubbleBase_01.glb");
    m_BubbleBaseModel->materials[0].shader =
        *ResourceManager::GetShader("shaders/bubbleBasic.vs", "shaders/bubbleBasic.fs");

    electroShieldRadius = GameData::electroShieldRadius;

    burstParticles = parentScene->CreateEntity<ParticleSystem>(true, 15);

    burstParticles->particleProperties.lifetime       = 0.2f;
    burstParticles->particleProperties.startSize      = 0.08f;
    burstParticles->particleProperties.endSize        = 0.005f;
    burstParticles->particleProperties.sizeVariation  = 0.05f;
    burstParticles->emitType                          = EmitType::BURST;
    burstParticles->shape                             = EmitShape::CIRCLE;
    burstParticles->particleProperties.initialSpeed   = 0.8f;
    burstParticles->particleProperties.speedVariation = 0.3f;
    burstParticles->particleProperties.damping        = 0.25f;
    burstParticles->particleProperties.startColor     = bubbleColors[0];
    burstParticles->particleProperties.endColor       = { 255, 255, 255, 0 };
}

void BubbleManager::OnEnable() {
    ResetInternal();
}

void BubbleManager::Update(float dT) {
    SpatialGrid::Clear();
    SpawnBubbles();

    // Process Bubbles
    for (int i = 0; i < m_Bubbles.size(); i++) {
        if (m_Bubbles[i] != nullptr && m_Bubbles[i]->isActive) {
            // Physics and Collisions
            UpdateBubble(m_Bubbles[i].get());

            // Check if Bubble was tapped
            for (int j = 0; j < GetTouchPointCount(); j++) {
                // LOGI("Touch Pos: %f, %f, %f", touchPos.x, touchPos.y, touchPos.z);
                if (IsPointInBubble(m_Bubbles[i].get(), Input::GetTouchPositionWS(j))) {
                    GameManager::Get().AddScore(5);
                    GameManager::Get().DecreaseSpawnInterval(0.01f);
                    GameManager::Get().AddSpecialBubbleInternal(m_Bubbles[i]->type);
                    GameCanvas::Get().ShowScorePop(5, GetWorldToScreen(m_Bubbles[i]->position, Game::Get().mainCamera3D));

                    burstParticles->position                      = m_Bubbles[i]->position;
                    burstParticles->particleProperties.startColor = bubbleColors[(int)m_Bubbles[i]->type];
                    burstParticles->particleProperties.endColor   = bubbleColors[(int)m_Bubbles[i]->type];
                    burstParticles->particleProperties.endColor.a = 0;

                    if (m_Bubbles[i]->type == ElementType::NONE)
                        burstParticles->shape = EmitShape::CIRCLE;
                    else {

                        Vector3 vecToSlots        = Vector3{ 0, 0, -27 } - m_Bubbles[i]->position;
                        burstParticles->direction = Vector3Normalize(vecToSlots) *
                            std::min(Vector3Length(vecToSlots), 5.f); //- Vector3{ 0, 25, 0 };
                        burstParticles->shape = EmitShape::NONE;
                    }

                    burstParticles->Burst(10);

                    m_Bubbles[i]->isActive = false;
                    break;
                }
            }

            if (EffectManager::Get().IsEffectActive(ElementType::ELECTRO)) {
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + electroShieldRadius) {
                    m_Bubbles[i]->isActive = false;
                }
            }

            // Check if bubble has reached center or strayed away
            if (m_Bubbles[i]->isActive) {
                //If bubbles have gone too far away
                if (Vector3Length(m_Bubbles[i]->position) > MAX_SPAWN_DIST + 5.f) {
                    m_Bubbles[i]->isActive = false;
                    continue;
                }

                //If bubbles have reached the center
                if (Vector3Length(m_Bubbles[i]->position) <= m_Bubbles[i]->radius + 2) {
                    if (m_Bubbles[i]->type == ElementType::NONE) {
                        GameManager::Get().DecreaseHealth(10);
                        GameCanvas::Get().ShowHealthPop(10);
                    }
                    m_Bubbles[i]->isActive = false;
                    continue;
                }
            }
        }
    }
    float time = GetTime();
    SetShaderValue(bubbleMaterials[(int)ElementType::NONE].shader, commonTimeId, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(bubbleMaterials[(int)ElementType::ELECTRO].shader, electroTimeId, &time, SHADER_UNIFORM_FLOAT);
    SetShaderValue(bubbleMaterials[(int)ElementType::ANEMO].shader, anemoTimeId, &time, SHADER_UNIFORM_FLOAT);
}

void BubbleManager::Draw() const {
    Color tint;
    for (size_t i = 0; i < m_Bubbles.size(); i++) {
        if (m_Bubbles[i] != nullptr && m_Bubbles[i]->isActive) {

            DrawModel(*m_BubbleBaseModel,
                m_Bubbles[i]->position,
                m_Bubbles[i]->radius,
                bubbleColors[(int)m_Bubbles[i]->type]);
        }
    }
}

bool BubbleManager::IsPointInBubble(Bubble* bubble, Vector3 point) const {
    return Vector3Length((Vector3){ point.x, 0, point.z } - bubble->position) <= bubble->radius;
}

/// returns position at a defined radius
Vector3 BubbleManager::GetRandomSpawnPos() {
    float distance  = GetRandomValue(MAX_SPAWN_DIST, MIN_SPAWN_DIST);
    float randAngle = GetRandomValue(0, 360);
    return (Vector3){ (float)cos(randAngle) * distance, 0, (float)sin(randAngle) * distance };
}
void BubbleManager::SpawnBubble(Bubble* bubble) {
    if (bubble == nullptr) {
        LOGW("Unable to spawn bubble : nullptr");
        return;
    }
    bubble->position = GetRandomSpawnPos();
    bubble->radius   = GetRandomValue(20, 25) / 10.f;
    bubble->velocity = Vector3Scale(Vector3Normalize(Vector3Zero() - bubble->position), bubble->CENTER_FORCE);

    if (RollPercentage(40)) {
        bubble->type = RollPercentage(50) ? ElementType::ANEMO : ElementType::ELECTRO;
    } else {
        bubble->type = ElementType::NONE;
    }
}
/// Update Bubble posiiton and check for collisions with other bubbles
void BubbleManager::UpdateBubble(Bubble* bubble) {
    bubble->velocity += Vector3Scale(Vector3Normalize(Vector3Zero() - bubble->position), bubble->CENTER_FORCE);
    bubble->ApplyForces();
    SpatialGrid::AddEntity(bubble);
    for (Bubble* nearby : SpatialGrid::GetNearbyEntities(bubble->position)) {
        if (CheckCollisionSpheres(bubble->position, bubble->radius, nearby->position, nearby->radius)) {
            bubble->ResolveCollision(nearby);
        }
    }
}
void BubbleManager::SpawnBubbles() {
    if (m_PauseSpawn) return;
    m_SpawnTimer += GetFrameTime();

    bool hasSpawned      = false;
    size_t activeBubbles = 0;

    if (m_SpawnTimer > s_SpawnInterval) {
        for (int i = 0; i < m_Bubbles.size(); i++) {
            if (!m_Bubbles[i]->isActive) {
                // LOGI("Bubble Spawned at frame: %f", GetTime());
                m_Bubbles[i]->isActive = true;
                // m_Bubbles[i]->Spawn();
                SpawnBubble(m_Bubbles[i].get());
                m_SpawnTimer = 0.0f;
                hasSpawned   = true;
                break;
            } else {
                activeBubbles++;
            }
        }
        if (!hasSpawned) {
            LOGE("Bubble Pool Exhausted, Active Bubbles: %zu", activeBubbles);
        }
    }
}
void BubbleManager::PauseSpawn() {
    m_PauseSpawn = true;
    LOGI("Bubble Spawn Paused");
}
void BubbleManager::ContinueSpawn() {
    m_PauseSpawn = false;
    LOGI("Bubble Spawn Resumed");
}
void BubbleManager::SpawnIntervalChanged(float spawnInterval, float amount) {
    s_SpawnInterval = spawnInterval;
}

void BubbleManager::ResetInternal() {
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i]->isActive = false;
    }
}
void BubbleManager::AnemoPushBack(bool active) {
    if (active) {
        Bubble::speedMultiplier = -5.f;
        PauseSpawn();

    } else {
        Bubble::speedMultiplier = 1.f;
        ContinueSpawn();
    }
}


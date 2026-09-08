#include "bubbleManager.h"
#include "EffectManager.h"
#include "ElementType.h"
#include "Game.h"
#include "GameCanvas.h"
#include "GameManager.h"
#include "Globals.h"
#include "LevelConfig.h"
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

#define PYRO_BURST_RADIUS 20.f

float Bubble::speedMultiplier = 1.f;

BubbleManager::BubbleManager(Scene* parentScene, LevelParams levelParams)
: DrawableEntity(parentScene, RenderQueue::OPAQUE)
, m_SpawnInterval(levelParams.startSpawnInterval)
, levelParams(levelParams) {
    m_Bubbles.resize(INITIAL_POOL_SIZE);
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i]           = std::make_unique<Bubble>();
        m_Bubbles[i]->isActive = false;
    }
    m_BubbleBaseModel = ResourceManager::GetModel("models/BubbleBase_01.glb");
    m_BubbleBaseModel->materials[0].shader =
        *ResourceManager::GetShader("shaders/bubbleBasic.vert", "shaders/bubbleBasic.frag");


    for (size_t i = 0; i < burstParticlesPool.size(); i++) {
        auto& burstParticles                         = burstParticlesPool[i];
        burstParticles                               = parentScene->CreateEntity<ParticleSystem>(true, 50);
        burstParticles->particleProperties.lifetime  = 0.45f;
        burstParticles->particleProperties.startSize = 0.025f;
        burstParticles->particleProperties.endSize   = 0.0f;
        burstParticles->particleProperties.sizeVariation  = 0.05f;
        burstParticles->emitType                          = EmitType::BURST;
        burstParticles->shape                             = EmitShape::CIRCLE;
        burstParticles->particleProperties.initialSpeed   = 2.0f;
        burstParticles->particleProperties.speedVariation = 0.3f;
        burstParticles->particleProperties.damping        = 0.45f;
        burstParticles->particleProperties.startColor     = bubbleColors[0];
        burstParticles->particleProperties.endColor       = { 255, 255, 255, 0 };

        burstParticles->endPoint      = { 0, 0, 0 };
        burstParticles->endPointForce = 0.7f;
    }

    LOGI("Bubble Manager constructed");
}

BubbleManager::~BubbleManager() {
}

void BubbleManager::Start() {
    electroShieldRadius = GameData::electroShieldRadius;
    Reset();
}

void BubbleManager::OnEnable() {
    Reset();
}

void BubbleManager::Update(float dT) {
    if (Globals::IsStateValid(Globals::GAMEPLAY_PAUSED)) return;

    SpatialGrid::Clear();
    SpawnBubbles();

    for (int i = 0; i < activeBubbleCount; i++) {
        if (activeBubbles[i] != nullptr) {
            // Update positions
            UpdateBubble(activeBubbles[i]);
        }
        // if (m_Bubbles[i] != nullptr && m_Bubbles[i]->isActive) {
        //     Bubble* bubble = m_Bubbles[i].get();
        // }
    }

    // Process Bubbles
    for (int i = 0; i < activeBubbleCount; i++) {
        if (activeBubbles[i] != nullptr) {
            Bubble* bubble     = activeBubbles[i];
            auto nearbyBubbles = SpatialGrid::GetNearbyEntities(bubble->position);

            for (int k = 0; k < nearbyBubbles.size(); k++) {
                Bubble* nearby = nearbyBubbles[k];
                if (CheckCollisionSpheres(bubble->position, bubble->radius, nearby->position, nearby->radius)) {
                    bubble->ResolveCollision(nearby);
                }
            }
            // Check if Bubble was tapped
            for (int j = 0; j < GetTouchPointCount(); j++) {
                // LOGI("Touch Pos: %f, %f, %f", touchPos.x, touchPos.y, touchPos.z);
                if (IsPointInBubble(bubble, Input::GetTouchRay(j))) {
                    BurstParticles(bubble);
                    DecreaseSpawnInterval();
                    GameManager::Get().AddSpecialBubbleInternal(bubble->type);
                    if (bubble->type == ElementType::NONE) {
                        GameManager::Get().AddScore();
                        GameCanvas::Get().ShowScorePop(GetWorldToScreen(bubble->position, Game::Get().mainCamera3D));
                    }
                    if (pyroBurstCharges > 0) {
                        int nearbyBubbleCount = 0;
                        for (int k = 0; k < nearbyBubbles.size(); k++) {
                            Bubble* nearby = nearbyBubbles[k];
                            if (Vector3Length(bubble->position - nearby->position) < PYRO_BURST_RADIUS) {
                                BurstParticles(nearby);
                                PopBubble(nearby->activeIndex);
                                nearbyBubbleCount++;
                            }
                        }
                        LOGI("BM: Nearby Bubbles found %d", nearbyBubbleCount);
                        if (--pyroBurstCharges <= 0) {
                            EffectManager::Get().DeactivateEffect(ElementType::PYRO);
                        }
                    }

                    PopBubble(i);
                    break;
                }
            }
            float distFromCenter = Vector3Length(bubble->position);

            if (EffectManager::Get().IsEffectActive(ElementType::ELECTRO)) {
                if (distFromCenter <= bubble->radius + electroShieldRadius) {
                    PopBubble(i);
                }
            }

            // Check if bubble has reached center or strayed away
            if (bubble->isActive) {
                // If bubbles have gone too far away
                if (distFromCenter > MAX_SPAWN_DIST + 5.f) {
                    PopBubble(i);
                    continue;
                }

                // If bubbles have reached the center
                if (distFromCenter <= bubble->radius + 2) {
                    if (bubble->type == ElementType::NONE) {
                        GameManager::Get().DecreaseHealth();
                        GameCanvas::Get().ShowHealthPop();
                    }
                    PopBubble(i);
                    continue;
                }
            }
        }
    }
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

// bool BubbleManager::IsPointInBubble(Bubble* bubble, Vector3 point) const {
//     return Vector3Length((Vector3){ point.x, 0, point.z } - bubble->position) <= bubble->radius;
// }

bool BubbleManager::IsPointInBubble(Bubble* bubble, Ray ray) const {
    RayCollision c = GetRayCollisionSphere(ray, bubble->position, bubble->radius);
    return c.hit;
    // return Vector3Length((Vector3){ point.x, 0, point.z } - bubble->position) <= bubble->radius;
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

    // SET Bubble type
    if (RollPercentage(levelParams.powerUpSpawnChance)) {
        // bubble->type = (ElementType)GetRandomValue(0, (int)ElementType::NONE);
        bubble->type = levelParams.availablePowerUps[GetRandomValue(0, levelParams.availablePowerUps.size() - 1)];
    } else {
        bubble->type = ElementType::NONE;
    }
}
/// Update Bubble posiiton and check for collisions with other bubbles
void BubbleManager::UpdateBubble(Bubble* bubble) {
    bubble->velocity += Vector3Scale(Vector3Normalize(Vector3Zero() - bubble->position), bubble->CENTER_FORCE);
    bubble->ApplyForces();
    SpatialGrid::AddEntity(bubble);
}
void BubbleManager::SpawnBubbles() {
    if (m_PauseSpawn) return;
    m_SpawnTimer += GetFrameTime();

    bool hasSpawned = false;
    // size_t activeBubbles = 0;

    if (m_SpawnTimer > m_SpawnInterval) {
        for (int i = 0; i < m_Bubbles.size(); i++) {
            if (!m_Bubbles[i]->isActive) {
                LOGV("Bubble Spawned at time: %f", GetTime());
                m_Bubbles[i]->isActive           = true;
                activeBubbles[activeBubbleCount] = m_Bubbles[i].get();
                activeBubbles[i]->activeIndex    = i;
                activeBubbleCount++;
                // m_Bubbles[i]->Spawn();
                SpawnBubble(m_Bubbles[i].get());
                m_SpawnTimer = 0.0f;
                hasSpawned   = true;
                break;
            }
        }
        if (!hasSpawned) {
            LOGE("Bubble Pool Exhausted, Active Bubbles: %d", activeBubbleCount);
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

void BubbleManager::Reset() {
    for (int i = 0; i < m_Bubbles.size(); i++) {
        m_Bubbles[i]->isActive = false;
    }
    std::fill(activeBubbles.begin(), activeBubbles.end(), nullptr);
    activeBubbleCount = 0;
    pyroBurstCharges  = 0;
    m_SpawnTimer      = 0.f;
    m_SpawnInterval   = levelParams.startSpawnInterval;
    m_PauseSpawn      = false;
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

void BubbleManager::CryoFreeze(bool active) {
    if (active) {
        Bubble::speedMultiplier = 0.1f;
        PauseSpawn();

    } else {
        Bubble::speedMultiplier = 1.f;
        ContinueSpawn();
    }
}

void BubbleManager::DecreaseSpawnInterval() {
    m_SpawnInterval -= levelParams.spawnDecrementAmount;
    m_SpawnInterval = std::max(m_SpawnInterval, levelParams.minSpawnInterval);
}
void BubbleManager::BurstParticles(Bubble* bubble) {
    int particleSystemIndex = 0;
    for (size_t i = 0; i < burstParticlesPool.size(); i++) {
        if (!burstParticlesPool[i]->isSimulating) {
            particleSystemIndex = i;
            break;
        }
    }
    // LOGI("BM: Using paricle system %d", particleSystemIndex);

    auto& burstParticles = burstParticlesPool[particleSystemIndex];

    burstParticles->position                      = bubble->position;
    burstParticles->particleProperties.startColor = bubbleColors[(int)bubble->type];
    burstParticles->particleProperties.endColor   = bubbleColors[(int)bubble->type];
    burstParticles->particleProperties.endColor.a = 0;

    if (bubble->type == ElementType::NONE || EffectManager::Get().IsEffectCharged(bubble->type)) {
        burstParticles->shape                       = EmitShape::CIRCLE;
        burstParticles->particleProperties.damping  = 0.45f;
        burstParticles->particleProperties.lifetime = 0.45f;
    } else {
        burstParticles->shape                       = EmitShape::LINE;
        burstParticles->particleProperties.lifetime = 1.25f;
        burstParticles->particleProperties.damping  = 0.25f;
    }

    burstParticles->Burst(50);
}
void BubbleManager::PopBubble(int index) {
    activeBubbles[index]->isActive = false;
    activeBubbles[index]           = activeBubbles[activeBubbleCount - 1];
    activeBubbleCount--;
}

#pragma once

#include "Entity.h"
#include "GameManager.h"
#include "Singleton.h"
#include "raylib.h"
#include <array>
#include <cstddef>
#include <memory>
#include <vector>

struct Bubble {
    bool isActive;
    ElementType type;
    Vector3 position;
    Vector3 velocity;
    float radius;

    constexpr static float maxMoveSpeed = 15;
    constexpr static float moveSpeed = 1.0;

    constexpr static const float DRAG = 0.1;
    ///Force multiplier  to be pulled towards center
    constexpr static const float CENTER_FORCE = 0.01;

    void ApplyForces()
    {
        velocity *= moveSpeed;
        if (Vector3Length(velocity) > maxMoveSpeed) {
            velocity = Vector3Normalize(velocity) * maxMoveSpeed;
        }

        velocity = Vector3Lerp(velocity, Vector3Zero(), DRAG);
        position += velocity;
    }
    ///Moves the collider and the bubble so that they dont intersect anymore
    void ResolveCollision(Bubble* collider)
    {
        Vector3 aMinusB = position - collider->position;
        float totalRadius = radius + collider->radius;
        float distance = Vector3Length(aMinusB);
        float penetration = totalRadius - distance;

        Vector3 collisionNormal = Vector3Normalize(aMinusB); /// distance);
        position += Vector3Scale(collisionNormal, penetration * 0.5f);
        collider->position -= Vector3Scale(collisionNormal, penetration * 0.5f);
    }
};

class BubbleManager : public DrawableEntity, public Singleton<BubbleManager> {
private:
    const int INITIAL_POOL_SIZE = 30;
    std::vector<std::unique_ptr<Bubble>> m_Bubbles;

    float m_SpawnTimer = 0.0f;
    bool m_PauseSpawn = false;

    void DoAnemoBlast(Bubble* bubble);

    void ResetInternal();

    static void ActiveEffectChanged(ElementType type);
    static void SpawnIntervalChanged(float spawnInterval, float amount);
    bool IsPointInBubble(Bubble* bubble, Vector3 point) const;

    ///Randomly Spawns bubbles in a circular ring based on spawn interval 
    void SpawnBubbles();
    ///activates and sets up bubble from the bubble pool
    void SpawnBubble(Bubble* bubble);
    Vector3 GetRandomSpawnPos();

    void UpdateBubble(Bubble* bubble);

    static ElementType s_ActiveEffect;
    static float s_SpawnInterval;

    std::shared_ptr<Model> m_BubbleBaseModel;
    std::array<Material, (size_t)ElementType::COUNT> bubbleMaterials; 
    int commonTimeId, electroTimeId, anemoTimeId;

public:
    BubbleManager();
    ~BubbleManager();

    void Start() override;
    void OnEnable() override;

    void Update(float dT = 1.0f) override;

    void Draw() const override;

    static void Reset() { Get().ResetInternal(); }

    void PauseSpawn();
    void ContinueSpawn();
};

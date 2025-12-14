#pragma once

#include "Entity.h"
#include "GameManager.h"
#include "Singleton.h"
#include "bubble.h"
#include <memory>
#include <vector>

class BubbleManager : public Entity, public Singleton<BubbleManager> {
private:
    const int m_MaxBubbleCount = 30;

    std::vector<std::unique_ptr<Bubble>> m_Bubbles;

    float m_SpawnTimer = 0.0f;
    bool m_PauseSpawn = false;

    void SpawnBubbleInternal();
    void DoAnemoBlast(Bubble* bubble);

    void ResetInternal();

    static void ActiveEffectChanged(ElementType type);
    static void SpawnIntervalChanged(float spawnInterval, float amount);

    static ElementType s_ActiveEffect;
    static float s_SpawnInterval;

public:
    BubbleManager();
    ~BubbleManager();

    void Start() override;
    void OnEnable() override;

    void Update(float dT = 1.0f) override;

    static void SpawnBubble() { Get().SpawnBubbleInternal(); }

    static void Reset() { Get().ResetInternal(); }

    void PauseSpawn();
    void ContinueSpawn();
};
// TODO: Add bubbles to scene

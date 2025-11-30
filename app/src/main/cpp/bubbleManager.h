#pragma once

#include "Entity.h"
#include "Singleton.h"
#include "bubble.h"
#include "gameData.h"
#include <memory>
#include <vector>

class BubbleManager : public Entity , public Singleton<BubbleManager>{
private:

    const int m_MaxBubbleCount = 30;

    std::vector<std::unique_ptr<Bubble>> m_Bubbles;

    float m_SpawnTimer = 0.0f;

    void SpawnBubbleInternal();
    void DoAnemoBlast(Bubble* bubble);

    void ResetInternal();

public:
    BubbleManager();
    ~BubbleManager();

    void Start() override;

    void Update(float dT = 1.0f) override;

    static void SpawnBubble() { Get().SpawnBubbleInternal(); }

    static void Reset() { Get().ResetInternal(); }
};
//TODO: Add bubbles to scene

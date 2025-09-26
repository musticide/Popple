#pragma once

#include "Entity.h"
#include "bubble.h"
#include "gameData.h"

class BubbleManager : public Entity {
private:
    BubbleManager();

    static BubbleManager& Get()
    {
        static BubbleManager instance;
        return instance;
    }

    const int m_MaxBubbleCount = 30;

    Bubble* m_Bubbles = new Bubble[m_MaxBubbleCount];

    float m_SpawnTimer = 0.0f;

    void SpawnBubbleInternal();
    void DoAnemoBlast(Bubble& bubble);

public:
    BubbleManager(BubbleManager&&) = delete;
    BubbleManager(const BubbleManager&) = delete;
    BubbleManager& operator=(BubbleManager&&) = delete;
    BubbleManager& operator=(const BubbleManager&) = delete;
    ~BubbleManager();

    void Start() override;

    void Update(float dT = 1.0f) override;

    static void SpawnBubble() { Get().SpawnBubbleInternal(); }

    static void Init();
};

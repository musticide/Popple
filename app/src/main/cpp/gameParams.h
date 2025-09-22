#pragma once

class GameParams {
private:
    GameParams();
    ~GameParams();

    static GameParams& Get()
    {
        static GameParams instance;
        return instance;
    }

    float m_SpawnInterval = 1.5f;
    float m_MinSpawnInterval = 0.20f;

    void SetSpawnIntervalInternal(float interval);

    void DecreaseSpawnIntervalInternal(float factor);

public:
    GameParams(GameParams&&) = delete;
    GameParams(const GameParams&) = delete;
    GameParams& operator=(GameParams&&) = delete;
    GameParams& operator=(const GameParams&) = delete;

    static float GetSpawnInterval() { return Get().m_SpawnInterval; }

    static void SetSpawnInterval(float interval) {Get().SetSpawnIntervalInternal(interval);}

    static void DecreaseSpawnInterval(float factor) {Get().DecreaseSpawnIntervalInternal(factor);}
};

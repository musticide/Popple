#pragma once

#include "Entity.h"
#include "Singleton.h"
#include "raylib.h"

class Score : public Entity, public Singleton<Score> {
private:
    int m_Score = 0;
    int m_Health = 100;

    void ShowScore() const;

    void ShowHealth() const;

    void AddScoreInternal(int score);

    void ResetScoreInternal();

    void DecreaseHealthInternal(int factor);

    void ResetHealthInternal();

public:
    Score();
    // static void ShowScore() { Get().ShowScoreInternal(); }

    static void AddScore(int score) { Get().AddScoreInternal(score); }

    static int GetScore() { return Get().m_Score; }

    static void ResetScore() { Get().ResetScoreInternal(); }

    static void DecreaseHealth(int factor) { Get().DecreaseHealthInternal(factor); }

    static int GetHealth() { return Get().m_Health; }

    static void ResetHealth() { Get().ResetHealthInternal(); }

    void Start() override;
    void Draw() const override;
};

#pragma once

#include "raylib.h"
class Score {
private:
    Score();
    ~Score();
    Score(Score const&) = delete;
    void operator=(Score const&) = delete;

    int m_Score = 0;
    int m_Health = 100;

    static Score& Get()
    {
        static Score instance;
        return instance;
    }

    void ShowScoreInternal() const;

    void AddScoreInternal(int score);

    void ResetScoreInternal();

    void ShowHealthInternal() const;

    void DecreaseHealthInternal(int factor);

    void ResetHealthInternal();

public:
    static void ShowScore() { Get().ShowScoreInternal(); }

    static void AddScore(int score) { Get().AddScoreInternal(score); }

    static int GetScore() {return Get().m_Score;}

    static void ResetScore() {Get().ResetScoreInternal();}

    static void ShowHealth() { Get().ShowHealthInternal();}

    static void DecreaseHealth(int factor) { Get().DecreaseHealthInternal(factor);}

    static int GetHealth() {return Get().m_Health;}

    static void ResetHealth() {Get().ResetHealthInternal();}
};

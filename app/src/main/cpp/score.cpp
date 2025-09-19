#include "score.h"
#include "raylib.h"
#include <string>

Score::Score() { }

Score::~Score() { }
void Score::ShowScoreInternal() const
{
    std::string scoreText = TextFormat("S: %d", m_Score);
    DrawText(scoreText.c_str(), -GetScreenWidth() / 2, -GetScreenHeight() / 2, 75, GRAY);
}

void Score::ResetScoreInternal() { m_Score = 0; }


void Score::AddScoreInternal(int score) { m_Score += score; }

void Score::ShowHealthInternal() const
{
    std::string healthText= TextFormat("H: %d", m_Health);
    int textWidth = MeasureText(healthText.c_str(), 75);
    DrawText(healthText.c_str(), (GetScreenWidth() / 2) - textWidth, -GetScreenHeight() / 2, 75, GRAY);
}

void Score::DecreaseHealthInternal(int factor) { m_Health -= factor; }

void Score::ResetHealthInternal() { m_Health = 100; }


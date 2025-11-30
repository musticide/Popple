#include "score.h"
#include "raylib.h"
#include <string>

void Score::ShowScore() const
{
    std::string scoreText = TextFormat("S: %d", m_Score);
    DrawText(scoreText.c_str(), -GetScreenWidth() / 2 + 40, -GetScreenHeight() / 2 + 40, 75, GRAY);
}

void Score::ResetScoreInternal() { m_Score = 0; }

void Score::AddScoreInternal(int score) { m_Score += score; }

void Score::ShowHealth() const
{
    std::string healthText = TextFormat("H: %d", m_Health);
    int textWidth = MeasureText(healthText.c_str(), 75);
    Color color = m_Health > 20 ? GREEN : RED;
    DrawText(healthText.c_str(), (GetScreenWidth() / 2) - textWidth - 40, -GetScreenHeight() / 2 + 40, 75, color);
}

void Score::DecreaseHealthInternal(int factor)
{
    if (m_Health > 0)
        m_Health -= factor;
}

void Score::ResetHealthInternal() { m_Health = 100; }

void Score::Draw() const
{
    ShowScore();
    ShowHealth();
}

void Score::Start() { this->SetRenderQueue(RenderQueue::UI); }

Score::Score() { SetRenderQueue(RenderQueue::UI); }


#include "GameUI.h"
#include "DrawableEntity.h"
#include "Entity.h"
#include "GameManager.h"
#include "Log.h"
#include <raymob.h>
#include <string>

int GameUI::s_Score;
int GameUI::s_Health;

GameUI::GameUI()
    : DrawableEntity(RenderQueue::UI)
{
}

GameUI::~GameUI() { }
void GameUI::Start()
{
    s_Health = GameManager::Get().GetHealth();
    s_Score = GameManager::Get().GetScore();
    GameManager::Get().scoreChanged.connect(GameUI::ScoreChanged);
    GameManager::Get().healthChanged.connect(GameUI::HealthChanged);
}

void GameUI::Update(float dT) { }

void GameUI::Draw() const
{
    // Show Score
    std::string scoreText = TextFormat("S: %d", s_Score);
    DrawText(scoreText.c_str(), -GetScreenWidth() / 2 + 40, -GetScreenHeight() / 2 + 40, 75, GRAY);

    // Show Health
    std::string healthText = TextFormat("H: %d", s_Health);
    int textWidth = MeasureText(healthText.c_str(), 75);
    Color color = s_Health > 20 ? GREEN : RED;
    DrawText(healthText.c_str(), (GetScreenWidth() / 2) - textWidth - 40, -GetScreenHeight() / 2 + 40, 75, color);

    // Show Combo UI
    for (int i = 0; i < GameManager::Get().GetMaxComboLength(); i++) {
        Color color = GRAY;
        float radius = 10;
        Vector2 center = { -80, (float)-GetScreenHeight() / 2 + 200 };
        center.x += 80 * i;

        for (int j = 1; j < (int)ElementType::COUNT; j++) {
            int comboCount = GameManager::Get().GetComboCountForType((ElementType)j);
            if (comboCount > 0 && i < comboCount) {
                color = GetElementalColor((ElementType)j);
                radius = 30;
            }
        }

        DrawCircleV(center, radius, color);
    }
}

Color GameUI::GetElementalColor(ElementType type) const
{
    Color result;

    switch (type) {
    case ElementType::NONE:
        result = WHITE;
        break;
    case ElementType::ELECTRO:
        result = PURPLE;
        break;
    case ElementType::ANEMO:
        result = { 107, 227, 183, 255 };
        break;
    default:
        result = WHITE;
        break;
    }

    return result;
}

#include "GameUI.h"
#include "Entity.h"
#include "Log.h"
#include "score.h"
#include <string>
#include "gameData.h"

GameUI::GameUI() {
    SetRenderQueue(RenderQueue::UI);
}

GameUI::~GameUI() {
}
void GameUI::Update(float dT) { }

void GameUI::Draw() const { 
    //Show Score
    std::string scoreText = TextFormat("S: %d", Score::GetScore());
    DrawText(scoreText.c_str(), -GetScreenWidth() / 2 + 40, -GetScreenHeight() / 2 + 40, 75, GRAY);

    //Show Health
    std::string healthText = TextFormat("H: %d", Score::GetHealth());
    int textWidth = MeasureText(healthText.c_str(), 75);
    Color color = Score::GetHealth() > 20 ? GREEN : RED;
    DrawText(healthText.c_str(), (GetScreenWidth() / 2) - textWidth - 40, -GetScreenHeight() / 2 + 40, 75, color);

    //Show Combo UI
    for (int i = 0; i < GameData::Get().MAX_COMBO_LENGTH; i++) {
        Color color = GRAY;
        float radius = 10;
        Vector2 center = { -80, (float)-GetScreenHeight() / 2 + 200 };
        center.x += 80 * i;

        for (int j = 1; j < BUBBLE_TYPES_COUNT; j++) {
            int comboCount = GameData::GetComboCountForType((ElementalEffect)j);
            if (comboCount  > 0 && i < comboCount ) {
                color = GameData::GetElementalColor((ElementalEffect)j);
                radius = 30;
            }
        }

        DrawCircleV(center, radius, color);
    }
}


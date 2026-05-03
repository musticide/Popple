#include "GameCanvas.h"
#include "GameManager.h"
#include "raylib.h"
#include "uiElement.h"
#include "uiText.h"
#include <string>

GameCanvas::GameCanvas() {
    scoreBox = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 35, 65, 350, 128 }, ui::FIXED_H | ui::FIXED_V);
    scoreBox->drawRect = { 0, 0, 350, 128 };

    scoreIcon = scoreBox->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 29, 19, 90, 90 }, ui::FIXED_H | ui::FIXED_V);
    scoreIcon->drawRect = { 112, 136, 90, 90 };

    scoreText = scoreBox->CreateChild<ui::Text>(true, Rectangle{ 138, 21, 184, 86 }, ui::FIXED_H | ui::FIXED_V);
    scoreText->hAlign = ui::ALIGN_CENTER;
    scoreText->vAlign = ui::ALIGN_MIDDLE;


    healthBox = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 688, 65, 350, 128 }, ui::FIXED_H | ui::FIXED_V);
    healthBox->drawRect = { 0, 0, -350, 128 };

    healthIcon = healthBox->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 231, 19, 90, 90 }, ui::FIXED_H | ui::FIXED_V);
    healthIcon->drawRect = { 0, 136, 90, 90 };

    healthText = healthBox->CreateChild<ui::Text>(true, Rectangle{ 17, 21, 184, 86 }, ui::FIXED_H | ui::FIXED_V);
    healthText->hAlign = ui::ALIGN_CENTER;
    healthText->vAlign = ui::ALIGN_MIDDLE;
}

GameCanvas::~GameCanvas() {
}
void GameCanvas::Start() {
    healthText->SetText(std::to_string(GameManager::Get().GetHealth()));
    scoreText->SetText(std::to_string(GameManager::Get().GetScore()));
    GameManager::Get().scoreChanged.connect([this](int s, int a) { ScoreChanged(s, a); });
    GameManager::Get().healthChanged.connect([this](int s, int a) { HealthChanged(s, a); });
    // GameManager::Get().healthChanged.connect(this->HealthChanged);
}
void GameCanvas::ScoreChanged(int score, int amount) {
    scoreText->SetText(std::to_string(score));
}

void GameCanvas::HealthChanged(int health, int amount) {
    scoreText->SetText(std::to_string(health));
}

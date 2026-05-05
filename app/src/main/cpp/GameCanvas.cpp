#include "GameCanvas.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "Log.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"
#include <stdbool.h>
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

    scorePopText = CreateElement<ui::Text>(false, Rectangle{ 0, 0, 100, 86 }, ui::FIXED_H | ui::FIXED_V);
    scoreText->hAlign = ui::ALIGN_LEFT;
    scorePopText->vAlign = ui::ALIGN_TOP;


    healthBox = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 688, 65, 350, 128 }, ui::FIXED_H | ui::FIXED_V);
    healthBox->drawRect = { 0, 0, -350, 128 };

    healthIcon = healthBox->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 231, 19, 90, 90 }, ui::FIXED_H | ui::FIXED_V);
    healthIcon->drawRect = { 0, 136, 90, 90 };

    healthText = healthBox->CreateChild<ui::Text>(true, Rectangle{ 17, 21, 184, 86 }, ui::FIXED_H | ui::FIXED_V);
    healthText->hAlign = ui::ALIGN_CENTER;
    healthText->vAlign = ui::ALIGN_MIDDLE;

    comboCircles[0] = CreateElement<ui::Image>(
        false, "textures/GameplayAtlas.png", Rectangle{ 376, 277, 90, 90 }, ui::FIXED_H | ui::FIXED_V);
    comboCircles[1] = CreateElement<ui::Image>(
        false, "textures/GameplayAtlas.png", Rectangle{ 494, 277, 90, 90 }, ui::FIXED_H | ui::FIXED_V);
    comboCircles[2] = CreateElement<ui::Image>(
        false, "textures/GameplayAtlas.png", Rectangle{ 612, 277, 90, 90 }, ui::FIXED_H | ui::FIXED_V);

    comboCircles[0]->drawRect = elementRects[(int)ElementType::NONE];
    comboCircles[1]->drawRect = elementRects[(int)ElementType::NONE];
    comboCircles[2]->drawRect = elementRects[(int)ElementType::NONE];

    electroShieldBtn = CreateElement<ui::Button>(
        true, "textures/GameplayAtlas.png", Rectangle{ 375, 2129, 130, 130 }, ui::FIXED_H | ui::FIXED_V);
    electroShieldBtn->onClick.connect([]() { EffectManager::Get().ActivateElectroShield(); });
    electroShieldBtn->drawRect = electroOffOnRect[0];

    electroChargeImg = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 368, 2072, 144, 57 }, ui::FIXED_H | ui::FIXED_V);
    electroChargeImg->drawRect = chargeCountRects[0];

    anemoShieldBtn = CreateElement<ui::Button>(
        true, "textures/GameplayAtlas.png", Rectangle{ 575, 2129, 130, 130 }, ui::FIXED_H | ui::FIXED_V);
    anemoShieldBtn->onClick.connect([]() { EffectManager::Get().ActivateAnemoShield(); });
    anemoShieldBtn->drawRect = anemoOffOnRect[0];

    anemoChargeImg = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 568, 2072, 144, 57 }, ui::FIXED_H | ui::FIXED_V);
    anemoChargeImg->drawRect = chargeCountRects[0];
}

GameCanvas::~GameCanvas() {
}
void GameCanvas::Start() {
    healthText->SetText(std::to_string(GameManager::Get().GetHealth()));
    scoreText->SetText(std::to_string(GameManager::Get().GetScore()));
    GameManager::Get().scoreChanged.connect([this](int s, int a) { ScoreChanged(s, a); });
    GameManager::Get().healthChanged.connect([this](int s, int a) { HealthChanged(s, a); });

    ui::Canvas::Start();
}

void GameCanvas::ScoreChanged(int score, int amount) {
    scoreText->SetText(std::to_string(score));
}

void GameCanvas::HealthChanged(int health, int amount) {
    healthText->SetText(std::to_string(health));
    if (health <= 20)
        healthText->color = RED;
    else
        healthText->color = WHITE;
}

void GameCanvas::Update(float dT) {
    // for (int i = 0; i < GameManager::Get().GetMaxComboLength(); i++) {
    //     comboCircles[i]->drawRect = elementRects[0];
    //     for (int j = 1; j < (int)ElementType::COUNT; j++) {
    //         int comboCount = GameManager::Get().GetComboCountForType((ElementType)j);
    //         if (comboCount > 0 && i < comboCount) {
    //             comboCircles[i]->drawRect = elementRects[j];
    //         }
    //     }
    // }

    electroShieldBtn->drawRect = electroOffOnRect[EffectManager::Get().electroShieldAvailable];
    if (!EffectManager::Get().electroShieldAvailable)
        electroChargeImg->drawRect = chargeCountRects[std::min(
            GameManager::Get().GetComboCountForType(ElementType::ELECTRO), GameManager::Get().GetMaxComboLength())];
    else
        electroChargeImg->drawRect = chargeCountRects[3];


    anemoShieldBtn->drawRect = anemoOffOnRect[EffectManager::Get().anemoShieldAvailable];
    if (!EffectManager::Get().anemoShieldAvailable)
        anemoChargeImg->drawRect = chargeCountRects[std::min(
            GameManager::Get().GetComboCountForType(ElementType::ANEMO), GameManager::Get().GetMaxComboLength())];
    else
        anemoChargeImg->drawRect = chargeCountRects[3];

    if (scorePopText->IsActive()) {
        // scorePopText->fRect.y += 10;
        // scorePopText->color.a -= dT;
        if ((scorePopTime -= dT) <= 0) {
            // scorePopText->SetActive(false);
            LOGI("Score pop false");
        }
    }


    ui::Canvas::Update(dT);
}
void GameCanvas::ShowScorePop(int score, Vector2 position) {
    scorePopText->SetText("+" + std::to_string(score));
    scorePopText->SetActive(true);
    scorePopText->fRect.x = position.x;
    scorePopText->fRect.y = position.y;
    scorePopTime          = 1.0f;
    LOGI("Score pop, %f, %f", position.x, position.y);
}

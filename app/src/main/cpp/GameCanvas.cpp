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

    for (auto& scorePopText : scorePopTexts) {
        scorePopText = CreateElement<ui::Text>(false, Rectangle{ 0, 0, 100, 86 }, ui::FIXED_H | ui::FIXED_V);
        scoreText->hAlign    = ui::ALIGN_LEFT;
        scorePopText->vAlign = ui::ALIGN_TOP;
    }

    for (auto& healthPopText : healthPopTexts) {
        healthPopText = CreateElement<ui::Text>(false, Rectangle{ 530, 1170, 100, 86 }, ui::FIXED_H | ui::FIXED_V);
        scoreText->hAlign     = ui::ALIGN_LEFT;
        healthPopText->vAlign = ui::ALIGN_TOP;
    }


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
    electroShieldBtn->onClick.connect([]() { EffectManager::Get().ActivateEffect(ElementType::ELECTRO); });
    electroShieldBtn->drawRect = electroOffOnRect[0];

    electroChargeImg = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 368, 2072, 144, 57 }, ui::FIXED_H | ui::FIXED_V);
    electroChargeImg->drawRect = chargeCountRects[0];

    anemoShieldBtn = CreateElement<ui::Button>(
        true, "textures/GameplayAtlas.png", Rectangle{ 575, 2129, 130, 130 }, ui::FIXED_H | ui::FIXED_V);
    anemoShieldBtn->onClick.connect([]() { EffectManager::Get().ActivateEffect(ElementType::ANEMO); });
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

    electroShieldBtn->drawRect = electroOffOnRect[EffectManager::Get().IsEffectCharged(ElementType::ELECTRO)];
    if (!EffectManager::Get().IsEffectCharged(ElementType::ELECTRO))
        electroChargeImg->drawRect =
            chargeCountRects[std::min(GameManager::Get().GetComboCountForType(ElementType::ELECTRO), GameData::MAX_COMBO_LENGTH)];
    else
        electroChargeImg->drawRect = chargeCountRects[3];


    anemoShieldBtn->drawRect = anemoOffOnRect[EffectManager::Get().IsEffectCharged(ElementType::ANEMO)];
    if (!EffectManager::Get().IsEffectCharged(ElementType::ANEMO))
        anemoChargeImg->drawRect =
            chargeCountRects[std::min(GameManager::Get().GetComboCountForType(ElementType::ANEMO), GameData::MAX_COMBO_LENGTH)];
    else
        anemoChargeImg->drawRect = chargeCountRects[3];

    for (auto& scorePopText : scorePopTexts) {
        if (scorePopText->IsActive()) {
            scorePopText->Move({ 0, -2 });
            scorePopText->color.a -= dT * 8.f;
            if ((scorePopTime -= dT) <= 0) {
                scorePopText->SetActive(false);
            }
        }
    }

    for (auto& healthPopText : healthPopTexts) {
        if (healthPopText->IsActive()) {
            healthPopText->Move({ 0, +2 });
            healthPopText->color.a -= dT * 8.f;
            if ((healthPopTime -= dT) <= 0) {
                healthPopText->SetActive(false);
            }
        }
    }

    ui::Canvas::Update(dT);
}

void GameCanvas::ShowScorePop(int score, Vector2 position) {
    for (auto& scorePopText : scorePopTexts) {
        if (scorePopText->IsActive()) continue;

        scorePopText->SetPosition(position);
        scorePopText->SetText("+" + std::to_string(score));
        scorePopText->color.a = 255;
        scorePopText->SetActive(true);
        scorePopTime = 0.5f;
        break;
    }
}

void GameCanvas::ShowHealthPop(int health) {
    for (auto& healthPopText : healthPopTexts) {
        if (healthPopText->IsActive()) continue;

        healthPopText->color = RED;
        healthPopText->SetText("-" + std::to_string(health));
        healthPopText->color.a = 255;
        healthPopText->SetActive(true);
        healthPopTime = 0.5f;
        break;
    }
}

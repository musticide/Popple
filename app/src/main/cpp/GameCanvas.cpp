#include "GameCanvas.h"
#include "EffectManager.h"
#include "ElementType.h"
#include "GameManager.h"
#include "LevelConfig.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"
#include <cstddef>
#include <stdbool.h>
#include <string>

GameCanvas::GameCanvas(Scene* parentScene, LevelParams params)
: ui::Canvas(parentScene)
, levelParams(params) {
    LOGI("Game Canvas Constructor");
    topPanel = CreateElement<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 0, 0, 1080, 256 }, ui::FIXED_W | ui::FIXED_H, true);
    topPanel->SetAnchor(ANCHOR_TOP_CENTER);
    topPanel->nPatchInfo.source = { 662, 1, 50, 50 };
    topPanel->nPatchInfo.layout = NPATCH_NINE_PATCH;
    topPanel->nPatchInfo.top    = 20;
    topPanel->nPatchInfo.bottom = 20;
    topPanel->nPatchInfo.left   = 20;
    topPanel->nPatchInfo.right  = 20;

    healthBar = topPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 652, 172, 400, 50 }, ui::FIXED_W | ui::FIXED_H, true);
    healthBar->SetAnchor(ANCHOR_TOP_RIGHT);
    healthBar->SetPivot(ANCHOR_MIDDLE_RIGHT);
    healthBar->nPatchInfo.source = { 560, 1, 50, 50 };
    healthBar->nPatchInfo.layout = NPATCH_NINE_PATCH;
    healthBar->nPatchInfo.top    = 20;
    healthBar->nPatchInfo.bottom = 20;
    healthBar->nPatchInfo.left   = 20;
    healthBar->nPatchInfo.right  = 20;

    healthBarBorder = topPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 652, 172, 400, 50 }, ui::FIXED_W | ui::FIXED_H, true);
    healthBarBorder->SetAnchor(ANCHOR_TOP_RIGHT);
    healthBarBorder->nPatchInfo.source = { 611, 1, 50, 50 };
    healthBarBorder->nPatchInfo.layout = NPATCH_NINE_PATCH;
    healthBarBorder->nPatchInfo.top    = 20;
    healthBarBorder->nPatchInfo.bottom = 20;
    healthBarBorder->nPatchInfo.left   = 20;
    healthBarBorder->nPatchInfo.right  = 20;

    scoreBar = topPanel->CreateChild<ui::Image>(
        !params.endlessMode, "textures/GameplayAtlas.png", Rectangle{ 20, 172, 400, 50 }, ui::FIXED_W | ui::FIXED_H, true);
    scoreBar->SetAnchor(ANCHOR_TOP_LEFT);
    scoreBar->SetPivot(ANCHOR_MIDDLE_LEFT);
    scoreBar->nPatchInfo.source = { 509, 1, 50, 50 };
    scoreBar->nPatchInfo.layout = NPATCH_NINE_PATCH;
    scoreBar->nPatchInfo.top    = 20;
    scoreBar->nPatchInfo.bottom = 20;
    scoreBar->nPatchInfo.left   = 20;
    scoreBar->nPatchInfo.right  = 20;

    scoreBarBorder = topPanel->CreateChild<ui::Image>(
        !params.endlessMode, "textures/GameplayAtlas.png", Rectangle{ 20, 172, 400, 50 }, ui::FIXED_W | ui::FIXED_H, true);
    scoreBarBorder->SetAnchor(ANCHOR_TOP_LEFT);
    scoreBarBorder->nPatchInfo.source = { 611, 1, 50, 50 };
    scoreBarBorder->nPatchInfo.layout = NPATCH_NINE_PATCH;
    scoreBarBorder->nPatchInfo.top    = 20;
    scoreBarBorder->nPatchInfo.bottom = 20;
    scoreBarBorder->nPatchInfo.left   = 20;
    scoreBarBorder->nPatchInfo.right  = 20;

    levelTxt = topPanel->CreateChild<ui::Text>(
        true, ui::ACE_BOLD, Rectangle{ params.endlessMode ? 20.f : 0.f, 120, 1080, 100 }, ui::FIXED_W | ui::FIXED_H);
    levelTxt->hAlign   = ui::ALIGN_CENTER;
    levelTxt->vAlign   = ui::ALIGN_BOTTOM;
    levelTxt->fontSize = 100;
    levelTxt->SetText(std::to_string(params.levelNumber));
    if (params.endlessMode) {
        levelTxt->hAlign   = ui::ALIGN_LEFT;
        levelTxt->vAlign   = ui::ALIGN_BOTTOM;
        levelTxt->fontSize = 72;
        levelTxt->SetText(std::to_string(params.levelNumber));
    }
    for (auto& scorePopText : scorePopTexts) {
        scorePopText =
            CreateElement<ui::Text>(false, ui::ACE_BOLD, Rectangle{ 0, 0, 100, 86 }, ui::FIXED_W | ui::FIXED_H);
        scorePopText->hAlign = ui::ALIGN_LEFT;
        scorePopText->vAlign = ui::ALIGN_TOP;
    }

    for (auto& healthPopText : healthPopTexts) {
        healthPopText =
            CreateElement<ui::Text>(false, ui::ACE_BOLD, Rectangle{ 530, 1170, 100, 86 }, ui::FIXED_W | ui::FIXED_H);
        healthPopText->hAlign = ui::ALIGN_LEFT;
        healthPopText->vAlign = ui::ALIGN_TOP;
    }

    bottomPanel = CreateElement<ui::UIElement>(true, Rectangle{ 0, 1952, 1080, 430 }, ui::FIXED_W | ui::FIXED_H);
    buttonsBgImg = bottomPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 0, 0, 540, 430 }, ui::FIXED_W | ui::FIXED_H, true);
    buttonsBgImg->SetAnchor(ANCHOR_BOTTOM_CENTER);
    buttonsBgImg->nPatchInfo.source = { 1, 855, 256, 168 };
    buttonsBgImg->nPatchInfo.layout = NPATCH_NINE_PATCH;
    buttonsBgImg->nPatchInfo.top    = 128;
    buttonsBgImg->nPatchInfo.bottom = 29;
    buttonsBgImg->nPatchInfo.left   = 20;
    buttonsBgImg->nPatchInfo.right  = 216;

    buttonsBgImg2 = bottomPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 540, 0, 540, 430 }, ui::FIXED_W | ui::FIXED_H, true);
    buttonsBgImg2->SetAnchor(ANCHOR_BOTTOM_CENTER);
    buttonsBgImg2->nPatchInfo.source = { 257, 855, 256, 168 };
    buttonsBgImg2->nPatchInfo.layout = NPATCH_NINE_PATCH;
    buttonsBgImg2->nPatchInfo.top    = 128;
    buttonsBgImg2->nPatchInfo.bottom = 29;
    buttonsBgImg2->nPatchInfo.left   = 216;
    buttonsBgImg2->nPatchInfo.right  = 20;

    comboCircles[0] = bottomPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 376, 28, 90, 90 }, ui::FIXED_W | ui::FIXED_H);
    comboCircles[1] = bottomPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 494, 28, 90, 90 }, ui::FIXED_W | ui::FIXED_H);
    comboCircles[2] = bottomPanel->CreateChild<ui::Image>(
        true, "textures/GameplayAtlas.png", Rectangle{ 612, 28, 90, 90 }, ui::FIXED_W | ui::FIXED_H);

    comboCircles[0]->nPatchInfo.source = elementRects[(int)ElementType::NONE];
    comboCircles[1]->nPatchInfo.source = elementRects[(int)ElementType::NONE];
    comboCircles[2]->nPatchInfo.source = elementRects[(int)ElementType::NONE];

    size_t buttonSpacing = 70;
    size_t buttonCount   = shieldButtons.size();
    float buttonSize     = 130;

    // total width of the row
    float totalWidth = buttonCount * buttonSize + (buttonCount - 1) * buttonSpacing;

    // leftmost starting point (so that it's centered)
    float startX = (1080 - totalWidth) * 0.5f;

    for (size_t i = 0; i < buttonCount; i++) {
        auto& button = shieldButtons[i];

        float x = startX + i * (buttonSize + buttonSpacing);

        Rectangle transform = Rectangle{ x, 177, buttonSize, buttonSize };

        button = bottomPanel->CreateChild<ui::Button>(
            true, "textures/GameplayAtlas.png", transform, ui::FIXED_W | ui::FIXED_H);
        button->onClick.connect([i]() { EffectManager::Get().ActivateEffect((ElementType)i); });
        button->nPatchInfo.source = offOnRects[i][0];
        button->tint              = LIGHTGRAY;
        button->clickable         = false;
        for (size_t j = 0; j < levelParams.availablePowerUps.size(); j++) {
            if (levelParams.availablePowerUps[j] == (ElementType)i) {
                button->tint      = WHITE;
                button->clickable = true;
                break;
            }
        }
    }
}

GameCanvas::~GameCanvas() {
    // Disconnect slots so GameManager never invokes a dangling lambda
    if (Singleton<GameManager>::IsInitialized()) {
        GameManager::Get().scoreChanged.disconnect(scoreConnId);
        GameManager::Get().healthChanged.disconnect(healthConnId);
    }
    LOGI("Game Canvas Destroyed");
}
void GameCanvas::Start() {
    scoreConnId  = GameManager::Get().scoreChanged.connect([this](int s, int a) { ScoreChanged(s, a); });
    healthConnId = GameManager::Get().healthChanged.connect([this](int h, int a) { HealthChanged(h, a); });
    ScoreChanged(GameManager::Get().GetScore(), 0);
    HealthChanged(GameManager::Get().GetHealth(), 0);

    ui::Canvas::Start();
}

void GameCanvas::ScoreChanged(int score, int amount) {
    if (levelParams.endlessMode) {
        if (score > PlayerProfile.levelsData.value[0].score) {
            levelTxt->SetText(std::to_string(score));
        } else {
            levelTxt->SetText(std::to_string(score) + "/" + std::to_string(PlayerProfile.levelsData.value[0].score));
        }
    } else {
        scoreBar->SetScale({ score / 500.f, 1.f });
    }
}

void GameCanvas::HealthChanged(int health, int amount) {
    healthBar->SetScale({ health / 100.f, 1.f });
}

void GameCanvas::Update(float dT) {
    for (int i = 0; i < GameData::MAX_COMBO_LENGTH; i++) {
        comboCircles[i]->nPatchInfo.source = elementRects[(int)ElementType::NONE];
        for (int j = 0; j < (int)ElementType::COUNT - 1; j++) {
            int comboCount = GameManager::Get().GetComboCountForType((ElementType)j);
            if (comboCount > 0 && i < comboCount) {
                comboCircles[i]->nPatchInfo.source = elementRects[j];
            }
        }
    }

    for (size_t i = 0; i < shieldButtons.size(); i++) {
        shieldButtons[i]->nPatchInfo.source = offOnRects[i][EffectManager::Get().IsEffectCharged((ElementType)i)];
    }
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

void GameCanvas::ShowScorePop(Vector2 position) {
    for (auto& scorePopText : scorePopTexts) {
        if (scorePopText->IsActive()) continue;

        scorePopText->SetPosition(position);
        scorePopText->SetText("+" + std::to_string(GameData::BUBBLE_POINTS));
        scorePopText->color.a = 255;
        scorePopText->SetActive(true);
        scorePopTime = 0.5f;
        break;
    }
}

void GameCanvas::ShowHealthPop() {
    for (auto& healthPopText : healthPopTexts) {
        if (healthPopText->IsActive()) continue;

        healthPopText->color = RED;
        healthPopText->SetText("-" + std::to_string(GameData::BUBBLE_DAMAGE));
        healthPopText->color.a = 255;
        healthPopText->SetActive(true);
        healthPopTime = 0.5f;
        break;
    }
}
void GameCanvas::Reset() {
    for (auto& scorePopText : scorePopTexts)
        scorePopText->SetActive(false);

    for (auto& healthPopText : healthPopTexts)
        healthPopText->SetActive(false);
}

void GameCanvas::OnEnable() {
    ui::Canvas::OnEnable();
    Reset();
}

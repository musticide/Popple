#include "LevelButtonsArray.h"
#include "LevelConfig.h"
#include "Log.h"
#include "RemoteConfig.h"
#include "uiCanvas.h"
#include "uiElement.h"

#define HIGHLIGHT_BTN_SCALE 1.3f

LevelButtonsArray::LevelButtonsArray(Scene* parentScene, ui::Canvas* parentCanvas, Rectangle rect, int fitType, int levelGroupNumber, LevelParams& levelParams)
: ui::UIElement(parentScene, parentCanvas, rect, fitType)
, levelParams(levelParams)
, levelGroupNumber(levelGroupNumber) {
    Vector2 spacing = { 62.f, 28.f };

    for (size_t i = 0; i < levelBtns.size(); i++) {
        size_t levelNumber = i + 1 + ((levelGroupNumber - 1) * MAX_LEVELS_IN_GROUP); // offset by the level group
        Rectangle rect = Rectangle{ 88, 316, 260, 260 };
        rect.x += (i % 3) * (rect.width + spacing.x);
        rect.y += std::floor(i / 3.f) * (rect.height + spacing.y);

        levelBtns[i] = CreateChild<LevelButton>(
            true, "textures/LevelsMenuAtlas.png", rect, std::to_string(levelNumber), ui::ACE_BOLD, ui::FIXED_W | ui::FIXED_H);

        levelBtns[i]->onClick.connect([this, levelNumber, i, &levelParams]() {
            levelParams = GetLevelParams(levelNumber);
            SetLevelButtonHighlighted(i);
        });
        levelBtns[i]->SetLevelNumber(levelNumber);
        levelBtns[i]->SetClickable(PlayerProfile.highestLevelCleared.value + 1 >= levelNumber);
        levelBtns[i]->SetScale((levelNumber == PlayerProfile.highestLevelCleared.value + 1) ? HIGHLIGHT_BTN_SCALE : 1.0f);
        levelBtns[i]->fontOffset.y = 90;
        levelBtns[i]->fontSize     = 90;
    }
}

LevelButtonsArray::~LevelButtonsArray() {
}

void LevelButtonsArray::SetLevelButtonHighlighted(int index) {
    if (index < 0 || index >= levelBtns.size()) return;

    for (size_t i = 0; i < levelBtns.size(); i++) {
        levelBtns[i]->SetScale((i == index) ? HIGHLIGHT_BTN_SCALE : 1.0f);
    }
}

void LevelButtonsArray::OnEnable() {
    ui::UIElement::OnEnable();
    Refresh();
}

void LevelButtonsArray::Refresh() {

    for (size_t i = 0; i < levelBtns.size(); i++) {
        size_t levelNumber = i + 1 + ((levelGroupNumber - 1) * MAX_LEVELS_IN_GROUP); // offset by the level group
        LevelButton* levelBtn = levelBtns[i].get();
        if (levelBtn != nullptr) {
            levelBtn->SetActive(true);
            levelBtn->onClick.connect([this, levelNumber]() { levelParams = GetLevelParams(levelNumber); });

            if (PlayerProfile.levelsData.value.size() >= levelNumber)
                levelBtn->SetRating(PlayerProfile.levelsData.value[levelNumber - 1].rating);
            else
                levelBtn->SetRating(0);

            levelBtn->SetLevelNumber(levelNumber);
            levelBtn->SetClickable(PlayerProfile.highestLevelCleared.value + 1 >= levelNumber);
            if (levelNumber == PlayerProfile.highestLevelCleared.value + 1) {
                levelBtn->SetScale(1.3f);
                levelParams = GetLevelParams(levelNumber);
            } else {
                levelBtn->SetScale(1.0f);
            }
        }
    }
}
void LevelButtonsArray::Start() {
    ui::UIElement::Start();

    for (size_t i = 0; i < levelBtns.size(); i++) {

        size_t levelNumber = i + 1 + ((levelGroupNumber - 1) * MAX_LEVELS_IN_GROUP); // offset by the level group
        levelBtns[i]->SetLevelNumber(levelNumber);
        if (PlayerProfile.levelsData.value.size() > i)
            levelBtns[i]->SetRating(PlayerProfile.levelsData.value[i].rating);
        else
            levelBtns[i]->SetRating(0);
    }
    Refresh();
}
void LevelButtonsArray::OnDisable() {
    ui::UIElement::OnDisable();
    for (size_t i = 0; i < levelBtns.size(); i++) {
        LevelButton* levelBtn = levelBtns[i].get();
        if (levelBtn != nullptr) {
            levelBtn->SetActive(false);
        }
    }
}
void LevelButtonsArray::SetTint(const Color& tint) {
    this->tint = tint;
    for (size_t i = 0; i < levelBtns.size(); i++) {
        LevelButton* levelBtn = levelBtns[i].get();
        if (levelBtn != nullptr) {
            levelBtn->SetTint(tint);
        }
    }
}


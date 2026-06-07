#include "LevelButton.h"
#include "Log.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"

LevelButton::LevelButton(Scene* parentScene, const char* filepath, Rectangle rect, int fitType)
: ui::Button(parentScene, filepath, rect, fitType) {
}

LevelButton::~LevelButton() {
}
void LevelButton::Start() {
    levelText = this->CreateChild<ui::Text>(
        true, ui::ROUNDED_MPLUS_EXTRABOLD, Rectangle{ 59, 86, 144, 72 }, ui::FIXED_H | ui::FIXED_V);
    levelText->hAlign = ui::ALIGN_CENTER;
    levelText->vAlign = ui::ALIGN_MIDDLE;
    levelText->SetText("");
    levelText->fontSize = 95;

    ratingImg = this->CreateChild<ui::Image>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 39, 8, 183, 78 }, ui::FIXED_H | ui::FIXED_V);
    ratingImg->drawRect = ratingRects[0];
}

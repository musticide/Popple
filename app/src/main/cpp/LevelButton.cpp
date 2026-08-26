#include "LevelButton.h"
#include "Log.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"

LevelButton::LevelButton(Scene* parentScene,
    ui::Canvas* parentCanvas,
    const char* filepath,
    Rectangle rect,
    std::string textContent,
    ui::FontName font,
    int fitType,
    bool nPatch)
: ui::Button(parentScene, parentCanvas, filepath, rect, textContent, font, fitType, nPatch) {
}
LevelButton::~LevelButton() {
}
void LevelButton::Start() {
    ui::Button::Start();

    ratingImg = this->CreateChild<ui::Image>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 39, 8, 183, 78 }, ui::FIXED_H | ui::FIXED_H);
    ratingImg->nPatchInfo.source = ratingRects[0];
}

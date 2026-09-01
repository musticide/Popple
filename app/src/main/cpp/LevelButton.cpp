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
    //NOTE: Some bug that is causing this to not generate from the constructor
    ratingImg = this->CreateChild<ui::Image>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 39, 8, 183, 78 }, ui::FIXED_H | ui::FIXED_H);
    ratingImg->nPatchInfo.source = ratingRects[0];
}

void LevelButton::OnEnable() {
    ui::Button::OnEnable();
    ratingImg->SetActive(true);
}

void LevelButton::OnDisable() {
    ui::Button::OnDisable();
    ratingImg->SetActive(false);
}

void LevelButton::SetTint(const Color& tint) {
    this->tint = tint;
    ratingImg->tint = tint;
    text->color = tint;
}


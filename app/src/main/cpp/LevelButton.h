#pragma once
#include "raylib.h"
#include "raymath.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>
#include <string>

#define MAX_RATING 4
class LevelButton : public ui::Button {
  public:
    LevelButton(Scene* parentScene,
        ui::Canvas* parentCanvas,
        const char* filepath,
        Rectangle rect,
        std::string textContent,
        ui::FontName font,
        int fitType,
        bool nPatch = false);
    ~LevelButton();
    void Start() override;
    void OnEnable() override;
    void OnDisable() override;

    void SetRating(int rating) {
        if (rating < MAX_RATING) {
            ratingImg->nPatchInfo.source = ratingRects[rating];
        }
    }
    void SetLevelNumber(int number) {
        if (text != nullptr) text->SetText(std::to_string(number));
    }

    std::unique_ptr<ui::Image> ratingImg = nullptr;

    void SetClickable(bool clickable) {
        nPatchInfo.source = buttonImageRect[clickable];
        this->clickable   = clickable;
    }

  private:
    Rectangle ratingRects[MAX_RATING] = {
        { 5, 519, 183, 78 },
        { 5, 436, 183, 78 },
        { 5, 353, 183, 78 },
        { 5, 270, 183, 78 },
    };
    Rectangle buttonImageRect[2] = {
        { 270, 5, 260, 260 },
        { 5, 5, 260, 260 },
    };
};

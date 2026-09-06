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

    void SetClickable(bool clickable, int levelGroupIndex) {
        nPatchInfo.source = clickable ? buttonImageRect[levelGroupIndex] : buttonLockedRect;
        this->clickable   = clickable;
    }

    void SetTint(const Color& tint);

  private:
    Rectangle ratingRects[MAX_RATING] = {
        { 5, 519, 183, 78 },
        { 5, 436, 183, 78 },
        { 5, 353, 183, 78 },
        { 5, 270, 183, 78 },
    };

    Rectangle buttonLockedRect    = { 266, 0, 260, 256 };

    Rectangle buttonImageRect[10] = { { 234, 256, 260, 256 },
        { 498, 256, 260, 256 },
        { 762, 256, 260, 256 },
        { 234, 512, 260, 256 },
        { 498, 512, 260, 256 },
        { 762, 512, 260, 256 },
        { 234, 768, 260, 256 },
        { 498, 768, 260, 256 },
        { 762, 768, 260, 256 },
        { 1, 1, 260, 256 } };
};

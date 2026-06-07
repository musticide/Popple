#pragma once
#include "raylib.h"
#include "uiButton.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>
#include <string>

#define MAX_RATING 4
class LevelButton : public ui::Button {
  public:
    LevelButton(const char* filepath, Rectangle rect, int fitType);
    LevelButton(LevelButton&&)                 = default;
    LevelButton(const LevelButton&)            = default;
    LevelButton& operator=(LevelButton&&)      = default;
    LevelButton& operator=(const LevelButton&) = default;
    ~LevelButton();
    void Start() override;

    void SetRating(int rating) {
        if (rating < MAX_RATING) {
            ratingImg->drawRect = ratingRects[rating];
        }
    }
    void SetLevelNumber(int number) {
        if (levelText != nullptr) levelText->SetText(std::to_string(number));
    }

  private:
    std::unique_ptr<ui::Image> ratingImg = nullptr;
    std::unique_ptr<ui::Text> levelText  = nullptr;

    Rectangle ratingRects[MAX_RATING] = {
        { 5, 519, 183, 78 },
        { 5, 436, 183, 78 },
        { 5, 353, 183, 78 },
        { 5, 270, 183, 78 },
    };
};

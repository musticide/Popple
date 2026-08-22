#pragma once
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <array>
#include <memory>

class EndGameCanvas : public ui::Canvas {
  public:
    EndGameCanvas(Scene* parentScene);
    EndGameCanvas(EndGameCanvas&&)                 = default;
    EndGameCanvas(const EndGameCanvas&)            = default;
    EndGameCanvas& operator=(EndGameCanvas&&)      = default;
    EndGameCanvas& operator=(const EndGameCanvas&) = default;
    ~EndGameCanvas();

    std::unique_ptr<ui::Image> bgImage                  = nullptr;
    std::array<std::unique_ptr<ui::Image>, 3> starImage = { 0 };
    std::unique_ptr<ui::Image> starBgGradient           = nullptr;
    std::unique_ptr<ui::Button> nextLevelButton         = nullptr;
    std::unique_ptr<ui::Button> retryLevelButton        = nullptr;
    std::unique_ptr<ui::Button> exitToMainButton        = nullptr;
    std::unique_ptr<ui::Text> resultText                = nullptr;
    std::unique_ptr<ui::Text> scoreText                 = nullptr;


  private:
    std::array<Rectangle, 2> starOnOffRect = { Rectangle{ 784, 4, 222, 222 }, Rectangle{ 558, 4, 222, 222 } };
};

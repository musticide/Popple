#pragma once
#include "LevelConfig.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <array>
#include <memory>

class EndGameCanvas : public ui::Canvas {
  public:
    EndGameCanvas(Scene* parentScene, LevelParams params);
    ~EndGameCanvas();

    std::unique_ptr<ui::Image> bgImage                  = nullptr;
    std::array<std::unique_ptr<ui::Image>, 3> starImages = { 0 };
    std::unique_ptr<ui::Image> starBgGradient           = nullptr;
    std::unique_ptr<ui::Button> nextLevelButton         = nullptr;
    std::unique_ptr<ui::Button> retryLevelButton        = nullptr;
    std::unique_ptr<ui::Button> exitToMainButton        = nullptr;
    std::unique_ptr<ui::Text> resultText                = nullptr;
    std::unique_ptr<ui::Text> scoreText                 = nullptr;

    void Start() override;
    void OnEnable() override;

    LevelParams levelParams;

  private:
    std::array<Rectangle, 2> starOnOffRect = { Rectangle{ 365, 546, 235, 235 }, Rectangle{ 365, 785, 235, 235 } };
    void Initialise();
};

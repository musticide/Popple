#pragma once
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>
#include <string>

class GameCanvas : public ui::Canvas {
  public:
    GameCanvas();
    GameCanvas(GameCanvas&&)            = default;
    GameCanvas& operator=(GameCanvas&&) = default;
    ~GameCanvas();

    std::unique_ptr<ui::Image> healthBox  = nullptr;
    std::unique_ptr<ui::Image> healthIcon = nullptr;
    std::unique_ptr<ui::Text> healthText  = nullptr;
    std::unique_ptr<ui::Image> scoreBox   = nullptr;
    std::unique_ptr<ui::Image> scoreIcon  = nullptr;
    std::unique_ptr<ui::Text> scoreText   = nullptr;

    void ScoreChanged(int score, int amount);
    void HealthChanged(int health, int amount);

    void Start() override;

  private:
};

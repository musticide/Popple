#pragma once
#include "Singleton.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>
#include <string>

class GameCanvas : public ui::Canvas, public Singleton<GameCanvas> {
  public:
    GameCanvas();
    ~GameCanvas();

    Rectangle elementRects[3] = {
        Rectangle{ 224, 136, 90, 90 }, // NONE
        Rectangle{ 381, 19, 90, 90 },  // ELECTRO
        Rectangle{ 336, 136, 90, 90 }  // ANEMO
    };

    Rectangle chargeCountRects[4] = { Rectangle{ 362, 0, 144, 57 }, Rectangle{ 362, 58, 144, 57 },
        Rectangle{ 362, 116, 144, 57 }, Rectangle{ 362, 174, 144, 57 } };

    Rectangle electroOffOnRect[2] = { Rectangle{ 0, 234, 130, 130 }, Rectangle{ 0, 371, 130, 130 } };
    Rectangle anemoOffOnRect[2]   = { Rectangle{ 136, 234, 130, 130 }, Rectangle{ 136, 371, 130, 130 } };

    std::unique_ptr<ui::Image> healthBox         = nullptr;
    std::unique_ptr<ui::Image> healthIcon        = nullptr;
    std::unique_ptr<ui::Text> healthText         = nullptr;
    std::unique_ptr<ui::Text> healthPopText      = nullptr;
    std::unique_ptr<ui::Image> scoreBox          = nullptr;
    std::unique_ptr<ui::Image> scoreIcon         = nullptr;
    std::unique_ptr<ui::Text> scoreText          = nullptr;
    std::unique_ptr<ui::Text> scorePopText       = nullptr;
    std::unique_ptr<ui::Image> comboCircles[3]   = { 0 }; // TODO: REMOVE THIS <<<<<
    std::unique_ptr<ui::Image> electroChargeImg  = nullptr;
    std::unique_ptr<ui::Button> electroShieldBtn = nullptr;
    std::unique_ptr<ui::Image> anemoChargeImg    = nullptr;
    std::unique_ptr<ui::Button> anemoShieldBtn   = nullptr;

    float scorePopTime = 0.0f, healthPopTime = 0.0f;

    void ScoreChanged(int score, int amount);
    void HealthChanged(int health, int amount);

    void Start() override;
    void Update(float dT = 1.0f) override;

    void ShowScorePop(int score, Vector2 position);
    void ShowHealthPop() {
    }

  private:
};

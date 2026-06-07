#pragma once
#include "GameManager.h"
#include "Singleton.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <array>
#include <memory>

typedef Rectangle OffOnRect[2];

class GameCanvas : public ui::Canvas, public Singleton<GameCanvas> {
  public:
    GameCanvas(Scene* parentScene);
    ~GameCanvas();

    Rectangle elementRects[(size_t)ElementType::COUNT] = {
        Rectangle{ 408, 316, 90, 90 }, // ELECTRO
        Rectangle{ 408, 218, 90, 90 }, // ANEMO
        Rectangle{ 408, 120, 90, 90 }, // CRY0
        Rectangle{ 408, 411, 90, 90 }, // NONE
    };

    // Rectangle electroOffOnRect[2] = { Rectangle{ 0, 234, 130, 130 }, Rectangle{ 0, 371, 130, 130 } };
    // Rectangle anemoOffOnRect[2]   = { Rectangle{ 136, 234, 130, 130 }, Rectangle{ 136, 371, 130, 130 } };
    // OffOnRect cryoOffOnRec        = { Rectangle{ 272, 234, 130, 130 }, Rectangle{ 272, 371, 130, 130 } };
    OffOnRect offOnRects[(int)ElementType::COUNT - 1] ={
     { Rectangle{ 0, 234, 130, 130 }, Rectangle{ 0, 371, 130, 130 } },
     { Rectangle{ 136, 234, 130, 130 }, Rectangle{ 136, 371, 130, 130 } },
     { Rectangle{ 272, 234, 130, 130 }, Rectangle{ 272, 371, 130, 130 } }
    };

    std::unique_ptr<ui::Image> healthBox                                           = nullptr;
    std::unique_ptr<ui::Image> healthIcon                                          = nullptr;
    std::unique_ptr<ui::Text> healthText                                           = nullptr;
    std::unique_ptr<ui::Text> healthPopText                                        = nullptr;
    std::unique_ptr<ui::Image> scoreBox                                            = nullptr;
    std::unique_ptr<ui::Image> scoreIcon                                           = nullptr;
    std::unique_ptr<ui::Text> scoreText                                            = nullptr;
    std::unique_ptr<ui::Image> comboCircles[3]                                     = { 0 };
    std::array<std::unique_ptr<ui::Button>, (int)ElementType::COUNT - 1> shieldButtons = { 0 };
    std::array<std::unique_ptr<ui::Text>, 3> scorePopTexts                         = { 0 };
    std::array<std::unique_ptr<ui::Text>, 3> healthPopTexts                        = { 0 };

    float scorePopTime = 0.0f, healthPopTime = 0.0f;

    void ScoreChanged(int score, int amount);
    void HealthChanged(int health, int amount);

    void Start() override;
    void Update(float dT = 1.0f) override;

    void ShowScorePop(int score, Vector2 position);
    void ShowHealthPop(int health);

  private:
};

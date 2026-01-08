#pragma once
#include "DrawableEntity.h"
#include "GameManager.h"
#include "raylib.h"

class GameUI : public DrawableEntity{
public:
    GameUI();
    GameUI(GameUI&&) = default;
    GameUI(const GameUI&) = default;
    GameUI& operator=(GameUI&&) = default;
    GameUI& operator=(const GameUI&) = default;
    ~GameUI();

    void Start() override;
    void Update(float dT = 1.0f) override;
    void Draw() const override;

    static void ScoreChanged(int score, int amount) { s_Score = score; }
    static void HealthChanged(int health, int amount) { s_Health = health; }

private:
    static int s_Score;
    static int s_Health;

    Color GetElementalColor(ElementType type) const;
};

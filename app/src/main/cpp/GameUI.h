#pragma once
#include "Entity.h"

class GameUI : public Entity{
public:
    GameUI();
    GameUI(GameUI &&) = default;
    GameUI(const GameUI &) = default;
    GameUI &operator=(GameUI &&) = default;
    GameUI &operator=(const GameUI &) = default;
    ~GameUI();

    void Update(float dT = 1.0f) override;
    void Draw() const override;

private:
};


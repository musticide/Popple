#pragma once
#include "Entity.h"
#include "LevelButton.h"
#include "LevelConfig.h"
#include "uiElement.h"
#include <array>


class LevelButtonsArray : public ui::UIElement {
  public:
    LevelButtonsArray(Scene* parentScene, ui::Canvas* parentCanvas, Rectangle rect, int fitType, int levelGroupNumber, LevelParams& levelParams);
    ~LevelButtonsArray();

    int levelGroupNumber;
    LevelParams& levelParams;
    std::array<std::unique_ptr<LevelButton>, 12> levelBtns = { 0 };

    void Start() override;
    void OnEnable() override;
    void OnDisable() override;
    void SetTint(const Color& tint);

    void Refresh();
  private:
    void SetLevelButtonHighlighted(int index);
};

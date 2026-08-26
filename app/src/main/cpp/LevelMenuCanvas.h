#pragma once
#include "LevelButton.h"
#include "LevelButtonsArray.h"
#include "LevelConfig.h"
#include "RemoteConfig.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include <array>
#include <memory>

class LevelMenuCanvas : public ui::Canvas {
  public:
    LevelMenuCanvas(Scene* parentScene);
    ~LevelMenuCanvas();
    std::unique_ptr<ui::Image> bgImage                                                 = nullptr;
    std::unique_ptr<ui::Button> playBtn                                                = nullptr;
    std::unique_ptr<ui::Button> backBtn                                                = nullptr;
    std::unique_ptr<ui::Button> nextBtn                                                = nullptr;
    std::unique_ptr<ui::Button> prevBtn                                                = nullptr;
    std::array<std::unique_ptr<LevelButtonsArray>, MAX_LEVEL_GROUPS> levelButtonsArray = { 0 };

    void Start() override;
    void OnEnable() override;
    void OnDisable() override;
    void Refresh();

  private:
    int activeLevelGroupIndex;
    LevelParams levelParams;
};

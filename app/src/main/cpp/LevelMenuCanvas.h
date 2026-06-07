#pragma once
#include "LevelConfig.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "LevelButton.h"
#include <array>
#include <memory>

class LevelMenuCanvas : public ui::Canvas {
  public:
    LevelMenuCanvas();
    ~LevelMenuCanvas();
    std::unique_ptr<ui::Image> bgImage = nullptr;
    std::array<std::unique_ptr<LevelButton>, 12> levelBtns = { 0 };
    std::unique_ptr<ui::Button> playBtn                  = nullptr;
    std::unique_ptr<ui::Button> backBtn                  = nullptr;

  private:

    LevelConfig m_LevelConfig;

    void Start() override;
};

#pragma once
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiButton.h"

class PauseGameCanvas : public ui::Canvas {
  public:
    PauseGameCanvas(Scene* parentScene);
    PauseGameCanvas(PauseGameCanvas&&)                 = default;
    PauseGameCanvas(const PauseGameCanvas&)            = default;
    PauseGameCanvas& operator=(PauseGameCanvas&&)      = default;
    PauseGameCanvas& operator=(const PauseGameCanvas&) = default;
    ~PauseGameCanvas();

    std::unique_ptr<ui::Image> darkBgImg  = nullptr;
    std::unique_ptr<ui::Button> resumeBtn = nullptr;
    std::unique_ptr<ui::Button> exitBtn   = nullptr;
    std::unique_ptr<ui::Text> msgTxt      = nullptr;


  private:
};

#pragma once
#include "UsernamePopup.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>

class HomeCanvas : public ui::Canvas {
  public:
    HomeCanvas(Scene* parentScene);
    ~HomeCanvas();

    std::unique_ptr<ui::Image> bgImage           = nullptr;
    std::unique_ptr<ui::Image> bgOverlayImg      = nullptr;
    std::unique_ptr<ui::Image> usernameBGImg     = nullptr;
    std::unique_ptr<ui::Button> classicModeBtn   = nullptr;
    std::unique_ptr<ui::Button> endlessModeBtn   = nullptr;
    std::unique_ptr<ui::Text> usernameTxt        = nullptr;
    std::unique_ptr<ui::Text> userLvlTxt         = nullptr;

    void Start() override;
    void OnEnable() override;
};

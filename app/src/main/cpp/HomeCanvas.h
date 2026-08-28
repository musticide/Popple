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
    HomeCanvas(HomeCanvas&&)                 = default;
    HomeCanvas(const HomeCanvas&)            = default;
    HomeCanvas& operator=(HomeCanvas&&)      = default;
    HomeCanvas& operator=(const HomeCanvas&) = default;
    ~HomeCanvas();

    std::unique_ptr<ui::Image> bgImage           = nullptr;
    std::unique_ptr<ui::Image> bgOverlayImg      = nullptr;
    std::unique_ptr<ui::Image> usernameBGImg     = nullptr;
    std::unique_ptr<ui::Button> startButton      = nullptr;
    std::unique_ptr<ui::Text> usernameTxt        = nullptr;
    std::unique_ptr<ui::Text> userLvlTxt         = nullptr;
    std::unique_ptr<UsernamePopup> usernamePopup = nullptr;

    void Start() override;
    void OnEnable() override;
};

#pragma once 
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>

class HomeCanvas : public ui::Canvas {
public:
    HomeCanvas();
    HomeCanvas(HomeCanvas &&) = default;
    HomeCanvas(const HomeCanvas &) = default;
    HomeCanvas &operator=(HomeCanvas &&) = default;
    HomeCanvas &operator=(const HomeCanvas &) = default;
    ~HomeCanvas();

    std::unique_ptr<ui::Image> bgImage = nullptr;
    std::unique_ptr<ui::Button> startButton = nullptr;
    std::unique_ptr<ui::Text> highScoreTxt = nullptr;
    std::unique_ptr<ui::Text> highestTimeTxt = nullptr;

    void OnEnable() override;
};


#pragma once

#include "HomeCanvas.h"
#include "Scene.h"
#include "Button.h"
#include <memory>
#include "TouchFeedback.h"
#include "uiCanvas.h"

class HomeScene : public Scene{
public:
    HomeScene();
    ~HomeScene();

    std::unique_ptr<TouchFeedback> touchFeedback = nullptr;
    std::unique_ptr<HomeCanvas> homeCanvas = nullptr;
private:
};


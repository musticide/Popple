#pragma once

#include "Scene.h"
#include "Button.h"
#include <memory>
#include "TouchFeedback.h"

class HomeScene : public Scene{
public:
    HomeScene();
    ~HomeScene();

    std::unique_ptr<Button> playButton = nullptr;
    std::unique_ptr<TouchFeedback> touchFeedback = nullptr;
    #ifdef DEBUG
    std::unique_ptr<Button> editorButton = nullptr;
    #endif // DEBUG
private:
};


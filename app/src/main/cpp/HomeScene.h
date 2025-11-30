#pragma once

#include "Scene.h"
#include "Button.h"
#include <memory>

class HomeScene : public Scene{
public:
    HomeScene();
    ~HomeScene();

    std::unique_ptr<Button> playButton = nullptr;
private:
    
};


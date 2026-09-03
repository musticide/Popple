#pragma once

#include "HomeCanvas.h"
#include "LevelMenuCanvas.h"
#include "Scene.h"
#include <memory>

class HomeScene : public Scene {
  public:
    HomeScene();
    ~HomeScene();

    std::unique_ptr<HomeCanvas> homeCanvas           = nullptr;
    std::unique_ptr<LevelMenuCanvas> levelMenuCanvas = nullptr;
    std::unique_ptr<UsernamePopup> usernamePopup = nullptr;


  private:
};

#pragma once
#include "Scene.h"
#include "LoadingCanvas.h"
#include <memory>

class LoadingScene : public Scene {
  public:
    LoadingScene();
    ~LoadingScene();

    void Load() {
    }

    std::unique_ptr<LoadingCanvas> loadingCanvas = nullptr;

  private:
};

#include "Scene.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>
class LoadingCanvas : public ui::Canvas {
  public:
    LoadingCanvas(Scene* parentScene);
    ~LoadingCanvas();

    std::unique_ptr<ui::Image> bgImage          = nullptr;
    std::unique_ptr<ui::Text> loadingPercentTxt = nullptr;

    void Update(float dT = 1.0f) override;
    // float loadingProgress = 0.f;
  private:
};

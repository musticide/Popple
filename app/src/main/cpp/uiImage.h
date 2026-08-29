#pragma once
#include "raylib.h"
#include "raymath.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include <memory>
#include <raymob.h>

namespace ui {

class Image : public UIElement {
  public:
    Image(Scene* parentScene,Canvas* parentCanvas, const char* filepath, Rectangle rect, int fitType, bool nPatch = false);
    ~Image();

    void Draw() const override;
    Color tint = WHITE;
    NPatchInfo nPatchInfo;

  private:
    bool isNPatch = false;
    std::shared_ptr<Texture2D> m_Texture;
};

} // namespace ui

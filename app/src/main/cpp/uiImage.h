#pragma once
#include "raylib.h"
#include "raymath.h"
#include "uiElement.h"
#include <memory>
#include <raymob.h>

namespace ui {

class Image : public UIElement {
  public:
    Image(Scene* parentScene, const char* filepath, Rectangle rect, int fitType, bool nPatch = false);
    Image(Image&&)                 = default;
    Image(const Image&)            = default;
    Image& operator=(Image&&)      = default;
    Image& operator=(const Image&) = default;
    ~Image();

    void Draw() const override;
    Color tint = WHITE;
    NPatchInfo nPatchInfo;

  private:
    bool isNPatch = false;
    std::shared_ptr<Texture2D> m_Texture;
};

} // namespace ui

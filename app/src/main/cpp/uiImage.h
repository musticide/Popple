#pragma once
#include "uiElement.h"
#include <memory>
#include <raymob.h>
#include "raymath.h"

namespace ui {

class Image : public UIElement{
  public:
    Image(Scene* parentScene, const char* filepath, Rectangle rect, int fitType);
    ~Image();

    void Draw() const override;
    Color tint = WHITE;
    float scale = 1.f;
    float rotation = 0.f;
    Vector2 pivot = Vector2Zero();

  private:
    std::shared_ptr<Texture2D> m_Texture;
};

} // namespace ui

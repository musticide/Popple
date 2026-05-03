#pragma once
#include "uiElement.h"
#include <memory>
#include <raymob.h>

namespace ui {

class Image : public UIElement{
  public:
    Image(const char* filepath, Rectangle rect, int fitType);
    ~Image();

    void Draw() const override;

  private:
    std::shared_ptr<Texture2D> m_Texture;
};

} // namespace ui

#pragma once
#include "Signal.h"
#include "raylib.h"
#include "uiElement.h"
#include "uiImage.h"
#include <memory>

namespace ui {
class Button : public ui::Image {
  public:
    Button(Scene* parentScene, const char* filepath, Rectangle rect, int fitType);
    Button(Button&&)                 = default;
    Button(const Button&)            = default;
    Button& operator=(Button&&)      = default;
    Button& operator=(const Button&) = default;
    ~Button();

    Signal<> onClick;

    void Update(float dT = 1.0f) override;
    ///TODO: Set raycast rect scaling
    Rectangle raycastRect;
    bool clickable = true;

  private:
    std::shared_ptr<Texture2D> m_Texture;
};
} // namespace ui

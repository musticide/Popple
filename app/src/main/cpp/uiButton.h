#pragma once
#include "Signal.h"
#include "raylib.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
#include <memory>

namespace ui {
class Button : public ui::Image {
  public:
    Button(Scene* parentScene, Canvas* parentCanvas, const char* filepath, Rectangle rect, int fitType, bool nPatch = false);
    Button(Scene* parentScene,
        Canvas* parentCanvas,
        const char* filepath,
        Rectangle rect,
        std::string textContent,
        FontName font,
        int fitType,
        bool nPatch = false);
    Button(Button&&)                 = default;
    Button(const Button&)            = delete;
    Button& operator=(Button&&)      = default;
    Button& operator=(const Button&) = delete;
    ~Button();

    Signal<> onClick;

    void Update(float dT = 1.0f) override;
    /// TODO: Set raycast rect scaling
    Rectangle raycastRect;
    bool clickable = true;

  private:
    bool wasPressed = false;

  public:
    std::unique_ptr<ui::Text> text = nullptr;

    void Start() override;

    int fontSize;
    Vector2 fontOffset = { 0, 0 };

  private:
    FontName font;
    std::string textContent;
};
} // namespace ui

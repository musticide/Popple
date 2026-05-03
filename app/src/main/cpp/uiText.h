#pragma once

#include "raylib.h"
#include "uiElement.h"

namespace ui {

enum TextAlignH {
    ALIGN_LEFT,
    ALIGN_CENTER,
    ALIGN_RIGHT
};
enum TextAlignV {

    ALIGN_TOP,
    ALIGN_MIDDLE,
    ALIGN_BOTTOM
};

class Text : public UIElement {
  public:
    Text(Rectangle rect, int fitType);
    Text(Text&&)                 = default;
    Text(const Text&)            = default;
    Text& operator=(Text&&)      = default;
    Text& operator=(const Text&) = default;
    ~Text();

    static Font aceBold;
    Color color       = WHITE;
    TextAlignH hAlign = ALIGN_LEFT;
    TextAlignV vAlign = ALIGN_BOTTOM;
    int fontSize      = 72;

    void Draw() const override;
    void SetText(std::string text) {
        this->text = text;
        textSize   = MeasureTextEx(aceBold, text.c_str(), (float)fontSize, 2);
        textPos    = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
               fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };
    }

  private:
    std::string text = "Sample Text";
    Vector2 textSize = MeasureTextEx(aceBold, text.c_str(), (float)fontSize, 2);
    Vector2 textPos  = { 0, 0 };
};

} // namespace ui

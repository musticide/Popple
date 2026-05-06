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

    void SetText(std::string text);

    void SetPosition(Vector2 pos);
    void SetPositionOffset(Vector2 pos);
    void Move(Vector2 pos);

  private:
    std::string text = "Sample Text";
    Vector2 textSize = MeasureTextEx(aceBold, text.c_str(), (float)fontSize, 2);
    Vector2 textPos = { 0, 0 }, textPosOffset = { 0, 0 };
};

} // namespace ui

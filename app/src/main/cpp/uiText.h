#pragma once

#include "raylib.h"
#include "uiCanvas.h"
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

//NOTE: When Adding fonts remember to increase the font array size
enum FontName {
    ACE_BOLD,
    ACE_BOLD_ITALIC,
    ROUNDED_MPLUS_EXTRABOLD
};
class Text : public UIElement {
  public:
    Text(Scene* parentScene,Canvas* parentCanvas, FontName font, Rectangle rect, int fitType);
    Text(const Text&)            = default;
    Text& operator=(Text&&)      = default;
    Text& operator=(const Text&) = default;
    ~Text();

    static std::array<Font, 3> fonts;
    FontName activeFont;
    // static Font aceBold;
    // static Font roundedMPlus_ExtraBold;
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
    Vector2 textSize = { 0, 0 };
    Vector2 textPos = { 0, 0 }, textPosOffset = { 0, 0 };

    void UpdateTextPos();
};

} // namespace ui

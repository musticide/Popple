#include "uiText.h"
#include "raylib.h"
#include "raymath.h"

// Font ui::Text::aceBold;
// Font ui::Text::roundedMPlus_ExtraBold;
std::array<Font, 2> ui::Text::fonts;

ui::Text::Text(Scene* parentScene, FontName font, Rectangle rect, int fitType)
: UIElement(parentScene, rect, fitType)
, activeFont(font) {
    if (!IsFontValid(fonts[ACE_BOLD])) fonts[ACE_BOLD] = LoadFontEx("fonts/AcephimereBold.otf", 72, 0, 250);
    if (!IsFontValid(fonts[ROUNDED_MPLUS_EXTRABOLD]))
        fonts[ROUNDED_MPLUS_EXTRABOLD] = LoadFontEx("fonts/MPLUSRounded1c-ExtraBold.ttf", 72, 0, 250);

    textPos = {rect.x, rect.y};
}

ui::Text::~Text() {
}

void ui::Text::Draw() const {

    DrawTextEx(fonts[activeFont], text.c_str(), textPos + textPosOffset, fontSize, 2, color);
}
void ui::Text::Move(Vector2 pos) {
    textPos += pos;
}

void ui::Text::SetPositionOffset(Vector2 pos) {
    textPosOffset = pos;
}

void ui::Text::SetPosition(Vector2 pos) {
    fRect.x = pos.x;
    fRect.y = pos.y;
    textPos = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
        fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };
}

void ui::Text::SetText(std::string text) {
    this->text = text;
    textSize   = MeasureTextEx(fonts[activeFont], text.c_str(), (float)fontSize, 2);
    textPos    = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
           fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };
}

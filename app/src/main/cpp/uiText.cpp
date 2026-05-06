#include "uiText.h"
#include "raylib.h"

Font ui::Text::aceBold;

ui::Text::Text(Rectangle rect, int fitType)
: UIElement(rect, fitType) {

    if (!IsFontValid(aceBold)) aceBold = LoadFontEx("fonts/AcephimereBold.otf", 72, 0, 250);
}

ui::Text::~Text() {
}

void ui::Text::Draw() const {

    DrawTextEx(aceBold, text.c_str(), textPos + textPosOffset, fontSize, 2, color);
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
    textSize   = MeasureTextEx(aceBold, text.c_str(), (float)fontSize, 2);
    textPos    = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
           fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };
}

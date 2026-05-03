#include "uiText.h"
#include "raylib.h"

Font ui::Text::aceBold;

ui::Text::Text(Rectangle rect, int fitType) : UIElement(rect, fitType) {

    if (!IsFontValid(aceBold))
        aceBold = LoadFontEx("fonts/AcephimereBold.otf", 72, 0, 250);
}

ui::Text::~Text() {
}

void ui::Text::Draw() const {
    Vector2 textPos = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
        fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };

    DrawTextEx(aceBold, text.c_str(), textPos, fontSize, 2, color);
}

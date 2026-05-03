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

    DrawTextEx(aceBold, text.c_str(), textPos, fontSize, 2, color);
}

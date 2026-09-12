#include "uiText.h"
#include "raylib.h"
#include "raymath.h"
#include "uiElement.h"

// Font ui::Text::aceBold;
// Font ui::Text::roundedMPlus_ExtraBold;
std::array<Font, 3> ui::Text::fonts;

ui::Text::Text(Scene* parentScene, Canvas* parentCanvas, FontName font, Rectangle rect, int fitType)
: UIElement(parentScene, parentCanvas, rect, fitType)
, activeFont(font) {
    if (!IsFontValid(fonts[ACE_BOLD])) fonts[ACE_BOLD] = LoadFontEx("fonts/AcephimereBold.otf", 72, 0, 250);
    if (!IsFontValid(fonts[ACE_BOLD_ITALIC]))
        fonts[ACE_BOLD_ITALIC] = LoadFontEx("fonts/AcephimereBoldItalic.otf", 72, 0, 250);
    if (!IsFontValid(fonts[ROUNDED_MPLUS_EXTRABOLD]))
        fonts[ROUNDED_MPLUS_EXTRABOLD] = LoadFontEx("fonts/MPLUSRounded1c-ExtraBold.ttf", 72, 0, 250);

    // UIElement's base constructor already ran UpdateFinalRect() (using the
    // base class's own virtual dispatch, since Text isn't fully constructed
    // yet), so fRect is already valid here -- derive textPos from that rather
    // than the raw, unscaled/unanchored constructor rect argument.
    UpdateTextPos();
}

ui::Text::~Text() {
}

void ui::Text::Draw() const {
    ui::UIElement::Draw();
    Vector2 scale = GetEffectiveScale();
    DrawTextEx(fonts[activeFont],
        text.c_str(),
        textPos + textPosOffset * scale,
        fontSize * scale.y,
        2,
        ColorTint(color, parentCanvas->canvasTint));
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
    UpdateTextPos();
}

void ui::Text::SetText(std::string text) {
    this->text  = text;
    Vector2 scale = GetEffectiveScale();
    textSize    = MeasureTextEx(fonts[activeFont], text.c_str(), (float)fontSize * scale.y, 2);
    UpdateTextPos();
}
void ui::Text::UpdateTextPos() {
    textPos = (Vector2){ fRect.x + Lerp(0.0f, fRect.width - textSize.x, ((float)hAlign) * 0.5f),
        fRect.y + Lerp(0.0f, fRect.height - textSize.y, ((float)vAlign) * 0.5f) };
}

Vector2 ui::Text::GetEffectiveScale() const {
    return { baseRect.width != 0.0f ? fRect.width / baseRect.width : 1.0f,
        baseRect.height != 0.0f ? fRect.height / baseRect.height : 1.0f };
}

void ui::Text::UpdateFinalRect() {
    ui::UIElement::UpdateFinalRect();
    UpdateTextPos();
}

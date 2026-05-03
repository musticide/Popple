#include "uiElement.h"
#include "DrawableEntity.h"
#include "raylib.h"
#include "uiCanvas.h"
using namespace ui;

UIElement::UIElement(Rectangle rect, int fitType)
: DrawableEntity(RenderQueue::UI), fitType(fitType), baseRect(rect) {
    SetFinalRectangle();
}

UIElement::~UIElement() {
}

void ui::UIElement::SetFinalRectangle() {
    // TODO:
    // Scale the rect and position it based on anchor
    if (parent) {
        fRect = { baseRect.x + parent->fRect.x, baseRect.y + parent->fRect.y, baseRect.width, baseRect.height };
    } else
        fRect = baseRect;
}


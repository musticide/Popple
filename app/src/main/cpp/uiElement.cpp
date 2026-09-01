#include "uiElement.h"
#include "DrawableEntity.h"
#include "Globals.h"
#include "Log.h"
#include "raylib.h"
#include "uiCanvas.h"
using namespace ui;

UIElement::UIElement(Scene* parentScene, Canvas* parentCanvas, Rectangle rect, int fitType)
: DrawableEntity(parentScene, RenderQueue::UI)
, fitType(fitType)
, baseRect(rect)
, parentCanvas(parentCanvas) {
    UpdateFinalRect();
}

UIElement::~UIElement() {
}


void ui::UIElement::UpdateFinalRect() {
    fRect = baseRect;

    // Convert to pixel space
    float pivotPixelX = fRect.x + (fRect.width * 0.5f);
    float pivotPixelY = fRect.y + (fRect.height * 0.5f);

    fRect.x = pivotPixelX + ((fRect.x - pivotPixelX) * scale * parentCanvas->canvasScale.x) +
        parentCanvas->canvasOffset.x;
    fRect.y = pivotPixelY + ((fRect.y - pivotPixelY) * scale * parentCanvas->canvasScale.y) +
        parentCanvas->canvasOffset.y;

    fRect.width *= scale * parentCanvas->canvasScale.x;
    fRect.height *= scale * parentCanvas->canvasScale.y;

    Vector2 globalScale = { Globals::uiScale.x, preserveRatio ? Globals::uiScale.x : Globals::uiScale.y };

    if (parent != nullptr) {
        fRect.x = (anchor.x * parent->fRect.width) +
            ((fRect.x - parent->baseRect.width * anchor.x) * globalScale.x * parent->scale);
        fRect.y = (anchor.y * parent->fRect.height) +
            ((fRect.y - parent->baseRect.height * anchor.y) * globalScale.y * parent->scale);

        fRect.width *= parent->scale;
        fRect.height *= parent->scale;

        fRect.x += parent->fRect.x;
        fRect.y += parent->fRect.y;


        if ((fitType & STRETCH_W) != 0) {
            // Preserve the right margin defined in the reference layout.
            const float rightMargin = parent->baseRect.width - (baseRect.x + baseRect.width);

            fRect.width = parent->fRect.width - baseRect.x - rightMargin;
        }

        if ((fitType & STRETCH_H) != 0) {
            // Preserve the bottom margin defined in the reference layout.
            const float bottomMargin = parent->baseRect.height - (baseRect.y + baseRect.height);

            fRect.height = parent->fRect.height - baseRect.y - bottomMargin;
        }
    } else {
        fRect.x =
            (anchor.x * Globals::screenWidth) + (fRect.x - Globals::baseScreenWidth * anchor.x) * globalScale.x;
        fRect.y = (anchor.y * Globals::screenHeight) +
            (fRect.y - Globals::baseScreenHeight * anchor.y) * globalScale.y;
        // Root elements use the actual drawable screen dimensions.
        if ((fitType & STRETCH_W) != 0) {
            const float widthScale =
                static_cast<float>(Globals::screenWidth) / static_cast<float>(Globals::baseScreenWidth);

            const float leftMargin = baseRect.x * widthScale;

            const float rightMargin = (Globals::baseScreenWidth - (baseRect.x + baseRect.width)) * widthScale;

            fRect.x     = leftMargin;
            fRect.width = static_cast<float>(Globals::screenWidth) - leftMargin - rightMargin;
        }

        if ((fitType & STRETCH_H) != 0) {
            const float heightScale =
                static_cast<float>(Globals::screenHeight) / static_cast<float>(Globals::baseScreenHeight);

            const float topMargin = baseRect.y * heightScale;

            const float bottomMargin = (Globals::baseScreenHeight - (baseRect.y + baseRect.height)) * heightScale;

            fRect.y      = topMargin;
            fRect.height = static_cast<float>(Globals::screenHeight) - topMargin - bottomMargin;
        }
    }
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] != nullptr) {
            children[i]->UpdateFinalRect();
        }
    }
}
void ui::UIElement::OnEnable() {
    UpdateFinalRect();
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] != nullptr) {
            children[i]->SetActive(true);
        }
    }
}

void ui::UIElement::OnDisable() {
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] != nullptr) {
            children[i]->SetActive(false);
        }
    }
}
void ui::UIElement::Draw() const {
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] != nullptr && children[i]->IsActive()) {
            children[i]->Draw();
        }
    }
    DrawableEntity::Draw();
}

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


Rectangle ui::UIElement::ComputeRect(Vector2 aniScaleOverride) const {
    Rectangle rect = baseRect;

    // Convert to pixel space, scaling around the configured pivot rather than
    // always assuming dead-center. NOTE: canvas pan/zoom is deliberately NOT
    // touched anywhere in this function -- it's applied once, uniformly, in
    // ApplyCanvasTransform, so it can't compound with nesting depth.
    float pivotPixelX = rect.x + (rect.width * pivot.x);
    float pivotPixelY = rect.y + (rect.height * pivot.y);

    Vector2 localScale = this->scale * aniScaleOverride;

    rect.x = pivotPixelX + ((rect.x - pivotPixelX) * localScale.x);
    rect.y = pivotPixelY + ((rect.y - pivotPixelY) * localScale.y);

    rect.width *= localScale.x;
    rect.height *= localScale.y;

    Vector2 globalScale = { Globals::uiScale.x, preserveRatio ? Globals::uiScale.x : Globals::uiScale.y };

    if (parent != nullptr) {
        // Use the parent's pure layout rect (pre-canvas-transform), not its
        // final fRect, so canvas pan/zoom doesn't get folded in here and then
        // folded in again by ApplyCanvasTransform on this element's own result.
        rect.x = (anchor.x * parent->m_LayoutRect.width) +
            ((rect.x - parent->baseRect.width * anchor.x) * globalScale.x * parent->scale.x *
                parent->aniScale.x);
        rect.y = (anchor.y * parent->m_LayoutRect.height) +
            ((rect.y - parent->baseRect.height * anchor.y) * globalScale.y * parent->scale.y *
                parent->aniScale.y);

        rect.width *= parent->scale.x * parent->aniScale.x;
        rect.height *= parent->scale.y * parent->aniScale.y;

        // Same trick as the root branch: x/y above are computed in screen
        // units, so width/height must be too, otherwise FIXED children stay in
        // design units and drift out of alignment when globalScale != 1.
        rect.width *= globalScale.x;
        rect.height *= globalScale.y;

        rect.x += parent->m_LayoutRect.x;
        rect.y += parent->m_LayoutRect.y;


        if ((fitType & STRETCH_W) != 0) {
            // Preserve the left/right margins defined in the reference layout,
            // scaled by the same factor as everything else.
            const float leftMargin  = baseRect.x * globalScale.x;
            const float rightMargin = (parent->baseRect.width - (baseRect.x + baseRect.width)) * globalScale.x;

            rect.width = parent->m_LayoutRect.width - leftMargin - rightMargin;
        }

        if ((fitType & STRETCH_H) != 0) {
            // Preserve the top/bottom margins defined in the reference layout,
            // scaled by the same factor as everything else.
            const float topMargin    = baseRect.y * globalScale.y;
            const float bottomMargin = (parent->baseRect.height - (baseRect.y + baseRect.height)) * globalScale.y;

            rect.height = parent->m_LayoutRect.height - topMargin - bottomMargin;
        }
    } else {
        rect.x =
            (anchor.x * Globals::screenWidth) + (rect.x - Globals::baseScreenWidth * anchor.x) * globalScale.x;
        rect.y = (anchor.y * Globals::screenHeight) +
            (rect.y - Globals::baseScreenHeight * anchor.y) * globalScale.y;
        // Keep FIXED sizes in the same (screen) coordinate space as the x/y
        // above -- without this, FIXED_W/FIXED_H leaves width/height in design
        // units while position is anchored to the real screen, which mixes
        // coordinate spaces and shifts parent/child rects out of alignment on
        // any window that isn't exactly the design resolution.
        rect.width *= globalScale.x;
        rect.height *= globalScale.y;
        // Root elements use the actual drawable screen dimensions.
        if ((fitType & STRETCH_W) != 0) {
            const float widthScale =
                static_cast<float>(Globals::screenWidth) / static_cast<float>(Globals::baseScreenWidth);

            const float leftMargin = baseRect.x * widthScale;

            const float rightMargin = (Globals::baseScreenWidth - (baseRect.x + baseRect.width)) * widthScale;

            rect.x     = leftMargin;
            rect.width = static_cast<float>(Globals::screenWidth) - leftMargin - rightMargin;
        }

        if ((fitType & STRETCH_H) != 0) {
            const float heightScale =
                static_cast<float>(Globals::screenHeight) / static_cast<float>(Globals::baseScreenHeight);

            const float topMargin = baseRect.y * heightScale;

            const float bottomMargin = (Globals::baseScreenHeight - (baseRect.y + baseRect.height)) * heightScale;

            rect.y      = topMargin;
            rect.height = static_cast<float>(Globals::screenHeight) - topMargin - bottomMargin;
        }
    }

    return rect;
}

Rectangle ui::UIElement::ApplyCanvasTransform(Rectangle rect) const {
    // A single, flat pan/zoom over the whole canvas, applied independently
    // and identically to every element's own pure layout rect. Because it's
    // an affine transform applied uniformly to every element in the same
    // coordinate space, nesting/containment is preserved automatically --
    // there's no need (and no correct way) to re-derive it per ancestor.
    rect.x = rect.x * parentCanvas->canvasScale.x + parentCanvas->canvasOffset.x;
    rect.y = rect.y * parentCanvas->canvasScale.y + parentCanvas->canvasOffset.y;
    rect.width *= parentCanvas->canvasScale.x;
    rect.height *= parentCanvas->canvasScale.y;
    return rect;
}

void ui::UIElement::UpdateFinalRect() {
    // Pure layout, no canvas transform -- children compute their own layout
    // from this, so canvas pan/zoom never compounds with nesting depth.
    m_LayoutRect = ComputeRect(this->aniScale);
    fRect        = ApplyCanvasTransform(m_LayoutRect);

    // Same rect, but with this element's own aniScale forced to identity, so a
    // press/pop animation never shrinks or grows the hit-test region under a
    // finger that hasn't actually moved.
    raycastRect = ApplyCanvasTransform(ComputeRect(Vector2{ 1.0f, 1.0f }));

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

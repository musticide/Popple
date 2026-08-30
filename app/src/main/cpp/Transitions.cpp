#include "Transitions.h"

void Transition::SlideInCanvas(ui::Canvas* from, ui::Canvas* to) {
    if (from == nullptr || to == nullptr) {
        LOGE("Could not transition. Invalid Canvas pointer");
        return;
    }
    to->SetActive(true);
    float val = 0.0f;
    TweenManager::Get().To(&val, 1000.f, 0.0f, 0.15f).SetEasing(Easing::CubicIn).OnUpdate([to](float x) {
        to->canvasOffset = { x, 0 };
        to->UpdateFinalRects();
    });
    Color tint = Color{ 255, 255, 255, 0 };
    TweenManager::Get()
        .To(&tint, tint, WHITE, 0.2f)
        .SetEasing(Easing::CubicIn)
        .OnUpdate([to](Color x) { to->canvasTint = x; })
        .OnCompleted([from]() { from->SetActive(false); });
}
void Transition::FadeOutTopCanvas(ui::Canvas* from, ui::Canvas* to) {
    if (from == nullptr || to == nullptr) {
        LOGE("Could not transition. Invalid Canvas pointer");
        return;
    }

    to->SetActive(true);
    Color tint = Color{ 255, 255, 255, 0 };
    TweenManager::Get()
        .To(&tint, WHITE, tint, 0.1f)
        .SetEasing(Easing::QuadInOut)
        .OnUpdate([from](Color x) { from->canvasTint = x; })
        .OnCompleted([from]() { from->SetActive(false); });
}

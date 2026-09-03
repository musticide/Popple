#include "Transitions.h"
#include "LevelButtonsArray.h"
#include "input.h"
#include "raylib.h"
#include "uiElement.h"

void Transition::SlideInCanvas(ui::Canvas* from, ui::Canvas* to, float time) {
    if (from == nullptr || to == nullptr) {
        LOGE("Could not transition. Invalid Canvas pointer");
        return;
    }
    to->SetActive(true);
    float val = 0.0f;
    Input::PauseInputForTime(time);
    TweenManager::Get().To(&val, 500.f, 0.0f, 0.2f).SetEasing(Easing::CubicIn).OnUpdate([to](float x) {
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

void Transition::FadeOutTopCanvas(ui::Canvas* from, ui::Canvas* to, float time) {
    if (from == nullptr || to == nullptr) {
        LOGE("Could not transition. Invalid Canvas pointer");
        return;
    }

    to->SetActive(true);
    Color tint = Color{ 255, 255, 255, 0 };
    Input::PauseInputForTime(time);
    TweenManager::Get()
        .To(&tint, WHITE, tint, 0.1f)
        .SetEasing(Easing::QuadInOut)
        .OnUpdate([from](Color x) { from->canvasTint = x; })
        .OnCompleted([from]() { from->SetActive(false); });
}

void Transition::SlideInLevelBtnsFromRight(LevelButtonsArray* element, float time) {
    if (element == nullptr) {
        LOGE("Could not transition. Invalid UiElement pointer");
        return;
    }
    float val           = element->baseRect.x;
    float start         = element->baseRect.x + 500;
    float end           = element->baseRect.x;
    element->baseRect.x = start;
    element->SetTint(Color{ 255, 255, 255, 0 });
    element->SetActive(true);
    Input::PauseInputForTime(time);
    TweenManager::Get().To(&val, start, end, time).SetEasing(Easing::CubicIn).OnUpdate([element](float x) {
        element->baseRect.x = x;
        element->parentCanvas->UpdateFinalRects();
    });
    Color tint = Color{ 255, 255, 255, 0 };
    TweenManager::Get().To(&tint, tint, WHITE, time).SetEasing(Easing::CubicIn).OnUpdate([element](Color x) {
        element->SetTint(x);
    });
}

void Transition::SlideInLevelBtnsFromLeft(LevelButtonsArray* element, float time) {
    if (element == nullptr) {
        LOGE("Could not transition. Invalid UiElement pointer");
        return;
    }
    float val           = element->baseRect.x;
    float start         = element->baseRect.x - 500;
    float end           = element->baseRect.x;
    element->baseRect.x = start;
    element->SetTint(Color{ 255, 255, 255, 0 });
    element->SetActive(true);

    Input::PauseInputForTime(time);
    TweenManager::Get().To(&val, start, end, time).SetEasing(Easing::CubicIn).OnUpdate([element](float x) {
        element->baseRect.x = x;
        element->parentCanvas->UpdateFinalRects();
    });

    Color tint = Color{ 255, 255, 255, 0 };
    TweenManager::Get().To(&tint, tint, WHITE, time).SetEasing(Easing::CubicIn).OnUpdate([element](Color x) {
        element->SetTint(x);
    });
}

void Transition::SlideOutLevelBtnsToLeft(LevelButtonsArray* element, float time) {
    if (element == nullptr) {
        LOGE("Could not transition. Invalid UiElement pointer");
        return;
    }

    float val   = element->baseRect.x;
    float start = element->baseRect.x;
    float end   = element->baseRect.x - 500;

    Input::PauseInputForTime(time);
    TweenManager::Get().To(&val, start, end, time).SetEasing(Easing::CubicIn).OnUpdate([element](float x) {
        element->baseRect.x = x;
        element->parentCanvas->UpdateFinalRects();
    });

    Color tint    = WHITE;
    Color endTint = Color{ 255, 255, 255, 0 };
    TweenManager::Get()
        .To(&tint, tint, endTint, time)
        .SetEasing(Easing::CubicIn)
        .OnUpdate([element](Color x) { element->SetTint(x); })
        .OnCompleted([element, start]() {
            element->SetActive(false);
            element->baseRect.x = start;
            element->SetTint(WHITE);
        });
}

void Transition::SlideOutLevelBtnsToRight(LevelButtonsArray* element, float time) {
    if (element == nullptr) {
        LOGE("Could not transition. Invalid UiElement pointer");
        return;
    }
    float val   = element->baseRect.x;
    float start = element->baseRect.x;
    float end   = element->baseRect.x + 500;

    Input::PauseInputForTime(time);
    TweenManager::Get().To(&val, start, end, time).SetEasing(Easing::CubicIn).OnUpdate([element](float x) {
        element->baseRect.x = x;
        element->parentCanvas->UpdateFinalRects();
    });

    Color tint    = WHITE;
    Color endTint = Color{ 255, 255, 255, 0 };
    TweenManager::Get()
        .To(&tint, tint, endTint, time)
        .SetEasing(Easing::CubicIn)
        .OnUpdate([element](Color x) { element->SetTint(x); })
        .OnCompleted([element, start]() {
            element->SetActive(false);
            element->baseRect.x = start;
            element->SetTint(WHITE);
        });
}

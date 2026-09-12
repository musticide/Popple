#include "uiButton.h"
#include "Log.h"
#include "input.h"
#include "raylib.h"
#include "uiCanvas.h"
#include "uiImage.h"
#include "uiText.h"
using namespace ui;

Button::Button(Scene* parentScene, Canvas* parentCanvas, const char* filepath, Rectangle rect, int fitType, bool nPatch)
: ui::Button(parentScene, parentCanvas, filepath, rect, "", ui::ACE_BOLD, fitType, nPatch) {
}

ui::Button::Button(Scene* parentScene,
    Canvas* parentCanvas,
    const char* filepath,
    Rectangle rect,
    std::string textContent,
    FontName font,
    int fitType,
    bool nPatch)
: Image(parentScene, parentCanvas, filepath, rect, fitType, nPatch) {
    this->font        = font;
    this->textContent = textContent;
    if (!textContent.empty()) {
        this->text = this->CreateChild<ui::Text>(true, font, Rectangle{ 0, 0, baseRect.width, baseRect.height }, fitType);
    }
}

Button::~Button() {
}

void ui::Button::Start() {
    if (text != nullptr) {
        text->baseRect = { 0, 0, baseRect.width, baseRect.height };
        text->SetPositionOffset(fontOffset);
        text->hAlign   = ALIGN_CENTER;
        text->vAlign   = ALIGN_TOP;
        text->fontSize = fontSize;
        text->UpdateFinalRect();
        text->SetText(textContent);
    }
}

void Button::Update(float dT) {
    if (!clickable) {
        state = PressState::None;
        SetAniScale(1.0f);
        return;
    }

    // Check if any touch is currently over the button
    bool touchingInside = false;
    bool touchingAtAll  = false;

    for (int i = 0; i < GetTouchPointCount(); i++) {
        touchingAtAll = true;
        if (CheckCollisionPointRec(Input::GetTouchPositionCS(i), raycastRect)) {
            touchingInside = true;
            break;
        }
    }

    switch (state) {
        case PressState::None:
            if (touchingInside) {
                state = PressState::PressedInside;
                SetAniScale(pressedScale); // finger just landed on the button
            }
            break;

        case PressState::PressedInside:
            if (!touchingAtAll) {
                // finger lifted while still over the button -> valid click
                state = PressState::None;
                SetAniScale(1.0f);
                onClick();
            } else if (!touchingInside) {
                // dragged off the button -> cancel, no click
                state = PressState::PressedOutside;
                SetAniScale(1.0f);
            }
            // else: still pressed inside, stay in this state
            break;

        case PressState::PressedOutside:
            if (!touchingAtAll) {
                // finger lifted off-button -> no click, just reset
                state = PressState::None;
            } else if (touchingInside) {
                // dragged back onto the button -> re-arm
                state = PressState::PressedInside;
                SetAniScale(pressedScale);
            }
            // else: still outside, stay in this state
            break;
    }
}

void ui::Button::OnEnable() {
    ui::Image::OnEnable();
    if (text != nullptr) {
        text->SetActive(true);
    }
}

void ui::Button::OnDisable() {
    ui::Image::OnDisable();
    if (text != nullptr) {
        text->SetActive(false);
    }
}

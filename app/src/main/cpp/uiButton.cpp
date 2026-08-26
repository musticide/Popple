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
    raycastRect       = fRect;
    this->font        = font;
    this->textContent = textContent;
}

Button::~Button() {
}


void ui::Button::Start() {
    if (!textContent.empty()) {
        this->text = this->CreateChild<ui::Text>(
            true, font, Rectangle{ fontOffset.x, fontOffset.y, baseRect.width, (float)fontSize }, fitType);
        text->hAlign   = ALIGN_CENTER;
        text->vAlign   = ALIGN_MIDDLE;
        text->fontSize = fontSize;
        text->SetText(textContent);
        // LOGI("Text Created %s", textContent.c_str());
    }
}

void Button::Update(float dT) {
    if (!clickable) {
        wasPressed = false;
        return;
    }

    // Check for presses (fire once per press, not every held frame)
    bool pressed = false;
    for (int i = 0; i < GetTouchPointCount(); i++) {
        if (CheckCollisionPointRec(Input::GetTouchPositionCS(i), fRect)) {
            pressed = true;
            break;
        }
    }

    if (pressed && !wasPressed) {
        wasPressed = true;
        onClick();
    } else if (!pressed) {
        wasPressed = false;
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



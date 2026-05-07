#include "uiButton.h"
#include "Log.h"
#include "input.h"
#include "raylib.h"
using namespace ui;

Button::Button(const char* filepath, Rectangle rect, int fitType) : Image(filepath, rect, fitType) {
}
Button::~Button() {
}

void Button::Update(float dT) {
    // Check for clicks
    for (int i = 0; i < GetTouchPointCount(); i++) {
        Vector2 touchPos = GetTouchPosition(i);
        // LOGI("Touch Pos: x %f, y %f", touchPos.x, touchPos.y);
        if (CheckCollisionPointRec(Input::GetTouchPositionCS(i), fRect)) {
            onClick();
            break;
        }
    }
}

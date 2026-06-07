#include "uiButton.h"
#include "Log.h"
#include "input.h"
#include "raylib.h"
using namespace ui;

Button::Button(Scene* parentScene, const char* filepath, Rectangle rect, int fitType)
: Image(parentScene, filepath, rect, fitType) {
    raycastRect = fRect;
}
Button::~Button() {
}

void Button::Update(float dT) {
    if (clickable) {
        // Check for clicks
        for (int i = 0; i < GetTouchPointCount(); i++) {
            Vector2 touchPos = GetTouchPosition(i);
            // LOGI("Touch Pos: x %f, y %f", touchPos.x, touchPos.y);
            if (CheckCollisionPointRec(Input::GetTouchPositionCS(i), raycastRect)) {
                onClick();
                break;
            }
        }
    }
}

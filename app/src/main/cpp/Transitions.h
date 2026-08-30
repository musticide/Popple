#pragma once
#include "Log.h"
#include "TweenManager.h"
#include "uiCanvas.h"
namespace Transition {

void SlideInCanvas(ui::Canvas* from, ui::Canvas* to);
void FadeOutTopCanvas(ui::Canvas* from, ui::Canvas* to);
} // namespace Transition

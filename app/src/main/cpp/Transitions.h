#pragma once
#include "LevelButtonsArray.h"
#include "Log.h"
#include "TweenManager.h"
#include "uiCanvas.h"
namespace Transition {

void SlideInCanvas(ui::Canvas* from, ui::Canvas* to, float time = 0.2f);
void FadeOutTopCanvas(ui::Canvas* from, ui::Canvas* to, float time = 0.1f);
void SlideInLevelBtnsFromRight(LevelButtonsArray* element, float time = 0.2f);
void SlideOutLevelBtnsToRight(LevelButtonsArray* element, float time = 0.2f);
void SlideInLevelBtnsFromLeft(LevelButtonsArray* element, float time = 0.2f);
void SlideOutLevelBtnsToLeft(LevelButtonsArray* element, float time = 0.2f);
} // namespace Transition

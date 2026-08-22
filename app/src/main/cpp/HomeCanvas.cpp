#include "HomeCanvas.h"
#include "Globals.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiText.h"
#include <cstdlib>
#include <string>

HomeCanvas::HomeCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);

    highScoreTxt = CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 540, 1080, 100 }, ui::FIXED_W | ui::FIXED_H);
    highScoreTxt->hAlign = ui::ALIGN_CENTER;
    highScoreTxt->vAlign = ui::ALIGN_MIDDLE;
    highScoreTxt->SetText("HighScore: " + std::to_string(PlayerProfile.highestScore.value));

    highestTimeTxt = CreateElement<ui::Text>(
        true, ui::ACE_BOLD_ITALIC, Rectangle{ 0, 680, 1080, 100 }, ui::FIXED_W | ui::FIXED_H);
    highestTimeTxt->hAlign = ui::ALIGN_CENTER;
    highestTimeTxt->vAlign = ui::ALIGN_MIDDLE;
    highestTimeTxt->SetText(TextFormat("Highest Time: %d", PlayerProfile.longestTimeSurvived.value));

    startButton = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 192, 958, 697, 350 }, "START", ui::ACE_BOLD_ITALIC, ui::FIXED_W | ui::FIXED_H, true);
    startButton->nPatchInfo.source = { 5, 694, 347, 325 };
    startButton->nPatchInfo.layout = NPATCH_NINE_PATCH;
    startButton->nPatchInfo.left   = 130;
    startButton->nPatchInfo.right  = 130;
    startButton->nPatchInfo.top    = 70;
    startButton->nPatchInfo.bottom = 170;
    startButton->fontOffset.y = 62;
    startButton->fontSize = 115;
}

HomeCanvas::~HomeCanvas() {
}
void HomeCanvas::OnEnable() {
    highScoreTxt->SetText("HighScore: " + std::to_string(PlayerProfile.highestScore.value));
    highestTimeTxt->SetText(TextFormat("Highest Time: %d", PlayerProfile.longestTimeSurvived.value));
}

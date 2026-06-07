#include "HomeCanvas.h"
#include "Globals.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiElement.h"
#include "uiText.h"
#include <cstdlib>
#include <string>
#include "PlayerProfile.h"

HomeCanvas::HomeCanvas() {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_H | ui::STRETCH_V);

    highScoreTxt = CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 540, 1080, 100 }, ui::FIXED_H | ui::FIXED_V);
    highScoreTxt->hAlign = ui::ALIGN_CENTER;
    highScoreTxt->vAlign = ui::ALIGN_MIDDLE;
    highScoreTxt->SetText("HighScore: " + std::to_string(PlayerProfile.highestScore.value));

    highestTimeTxt= CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 680, 1080, 100 }, ui::FIXED_H | ui::FIXED_V);
    highestTimeTxt->hAlign = ui::ALIGN_CENTER;
    highestTimeTxt->vAlign = ui::ALIGN_MIDDLE;
    highestTimeTxt->SetText(TextFormat("Highest Time: %d", PlayerProfile.longestTimeSurvived.value));

    startButton = CreateElement<ui::Button>(
        true, "textures/StartButton.png", Rectangle{ 223, 981, 616, 279 }, ui::FIXED_H | ui::FIXED_V);
}

HomeCanvas::~HomeCanvas() {
}
void HomeCanvas::OnEnable() {
    highScoreTxt->SetText("HighScore: " + std::to_string(PlayerProfile.highestScore.value));
    highestTimeTxt->SetText(TextFormat("Highest Time: %d", PlayerProfile.longestTimeSurvived.value));
}

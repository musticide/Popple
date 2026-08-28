#include "HomeCanvas.h"
#include "Globals.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"
#include <cstdlib>
#include <string>

HomeCanvas::HomeCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);

    bgOverlayImg = CreateElement<ui::Image>(
        true, "textures/MainMenuBGOverlay.png", Rectangle{ 0, 1161, 1080, 1175}, ui::STRETCH_W | ui::STRETCH_H);

    usernameBGImg = CreateElement<ui::Image>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 36, 41, 994, 157 }, ui::FIXED_H | ui::STRETCH_W, true);
    usernameBGImg->nPatchInfo.source = { 222, 286, 350, 157 };
    usernameBGImg->nPatchInfo.layout = NPATCH_THREE_PATCH_HORIZONTAL;
    usernameBGImg->nPatchInfo.left   = 230;
    usernameBGImg->nPatchInfo.right  = 48;

    usernameTxt = CreateElement<ui::Text>(
        true, ui::ROUNDED_MPLUS_EXTRABOLD, Rectangle{ 421, 48, 581, 143 }, ui::FIXED_H | ui::FIXED_W);
    usernameTxt->hAlign   = ui::ALIGN_RIGHT;
    usernameTxt->vAlign   = ui::ALIGN_MIDDLE;
    usernameTxt->fontSize = 72;
    usernameTxt->SetText(PlayerProfile.username.value);

    userLvlTxt = CreateElement<ui::Text>(
        true, ui::ROUNDED_MPLUS_EXTRABOLD, Rectangle{ 188, 48, 690, 143 }, ui::FIXED_H | ui::FIXED_W);
    userLvlTxt->hAlign   = ui::ALIGN_LEFT;
    userLvlTxt->vAlign   = ui::ALIGN_MIDDLE;
    userLvlTxt->fontSize = 96;
    userLvlTxt->SetText(std::to_string(PlayerProfile.highestLevelCleared.value));

    startButton = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 192, 958, 697, 350 }, "START", ui::ACE_BOLD_ITALIC, ui::FIXED_W | ui::FIXED_H, true);
    startButton->nPatchInfo.source = { 5, 694, 347, 325 };
    startButton->nPatchInfo.layout = NPATCH_NINE_PATCH;
    startButton->nPatchInfo.left   = 130;
    startButton->nPatchInfo.right  = 130;
    startButton->nPatchInfo.top    = 70;
    startButton->nPatchInfo.bottom = 170;
    startButton->fontOffset.y      = 62;
    startButton->fontSize          = 115;

    usernamePopup = CreateElement<UsernamePopup>(true, Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);
}

HomeCanvas::~HomeCanvas() {
}
void HomeCanvas::OnEnable() {
    usernameTxt->SetText(PlayerProfile.username.value);
    LOGI("Username: %s", PlayerProfile.username.value.c_str());
    userLvlTxt->SetText(std::to_string(PlayerProfile.highestLevelCleared.value));
    // highScoreTxt->SetText("HighScore: " + std::to_string(PlayerProfile.highestScore.value));
    // highestTimeTxt->SetText(TextFormat("Highest Time: %d", PlayerProfile.longestTimeSurvived.value));
}
void HomeCanvas::Start() {
    ui::Canvas::Start();
    usernamePopup->SetActive(false);
}

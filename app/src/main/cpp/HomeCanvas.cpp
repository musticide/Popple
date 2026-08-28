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
        true, "textures/MainMenuBGOverlay.png", Rectangle{ 0, 1161, 1080, 1175 }, ui::STRETCH_W | ui::STRETCH_H);

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

    classicModeBtn                    = CreateElement<ui::Button>(true,
        "textures/LevelsMenuAtlas.png",
        Rectangle{ 176, 891, 727, 326 },
        "Classic",
        ui::ACE_BOLD_ITALIC,
        ui::FIXED_W | ui::FIXED_H,
        true);
    classicModeBtn->nPatchInfo.source = { 5, 694, 347, 325 };
    classicModeBtn->nPatchInfo.layout = NPATCH_NINE_PATCH;
    classicModeBtn->nPatchInfo.left   = 130;
    classicModeBtn->nPatchInfo.right  = 130;
    classicModeBtn->nPatchInfo.top    = 70;
    classicModeBtn->nPatchInfo.bottom = 170;
    classicModeBtn->fontOffset.y      = 62;
    classicModeBtn->fontSize          = 115;

    endlessModeBtn                    = CreateElement<ui::Button>(true,
        "textures/LevelsMenuAtlas.png",
        Rectangle{ 176, 1187, 727, 326 },
        "Endless",
        ui::ACE_BOLD_ITALIC,
        ui::FIXED_W | ui::FIXED_H,
        true);
    endlessModeBtn->nPatchInfo.source = { 5, 694, 347, 325 };
    endlessModeBtn->nPatchInfo.layout = NPATCH_NINE_PATCH;
    endlessModeBtn->nPatchInfo.left   = 130;
    endlessModeBtn->nPatchInfo.right  = 130;
    endlessModeBtn->nPatchInfo.top    = 70;
    endlessModeBtn->nPatchInfo.bottom = 170;
    endlessModeBtn->fontOffset.y      = 62;
    endlessModeBtn->fontSize          = 115;
    endlessModeBtn->tint = LIGHTGRAY;
    endlessModeBtn->text->color = LIGHTGRAY;
    endlessModeBtn->clickable = false;

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

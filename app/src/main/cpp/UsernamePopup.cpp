#include "UsernamePopup.h"
#include "PlayerProfile.h"
#include "raylib.h"
#include "raymob.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"

UsernamePopup::UsernamePopup(Scene* parentScene)
: ui::Canvas(parentScene){
    darkBgImg = CreateElement<ui::Image>(
        true, "textures/Empty.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);
    darkBgImg->tint = Color{ 0, 0, 0, 200 };

    usernameBgImg = CreateElement<ui::Image>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 36, 869, 994, 157 }, ui::FIXED_H | ui::STRETCH_W, true);
    usernameBgImg->nPatchInfo.source = { 222, 286, 350, 157 };
    usernameBgImg->nPatchInfo.layout = NPATCH_THREE_PATCH_HORIZONTAL;
    usernameBgImg->nPatchInfo.left   = 230;
    usernameBgImg->nPatchInfo.right  = 48;

    confirmBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 564, 1068, 157, 157 }, ui::FIXED_H | ui::STRETCH_W);
    confirmBtn->nPatchInfo.source = { 415, 475, 157, 157 };
    confirmBtn->onClick.connect([this]() {
        if (usernameBuffer[0] != '\0') {
            PlayerProfile.username.value = usernameBuffer;
            SyncMetaData();
            this->SetActive(false);
        }
    });

    cancelBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 349, 1068, 157, 157 }, ui::FIXED_H | ui::STRETCH_W);
    cancelBtn->nPatchInfo.source = { 222, 475, 157, 157 };
    cancelBtn->onClick.connect([this]() { usernameBuffer[0] = '\0'; });

    msgTxt = CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 476, 1080, 228 }, ui::FIXED_H | ui::STRETCH_W);
    msgTxt->hAlign   = ui::ALIGN_CENTER;
    msgTxt->vAlign   = ui::ALIGN_MIDDLE;
    msgTxt->fontSize = 96;
    msgTxt->SetText("ENTER NEW\nUSERNAME");

    usernameTxt = CreateElement<ui::Text>(
        true, ui::ROUNDED_MPLUS_EXTRABOLD, Rectangle{ 421, 869, 581, 157 }, ui::FIXED_H | ui::FIXED_W);
    usernameTxt->hAlign   = ui::ALIGN_RIGHT;
    usernameTxt->vAlign   = ui::ALIGN_MIDDLE;
    usernameTxt->fontSize = 72;
}
UsernamePopup::~UsernamePopup() {
}
void UsernamePopup::OnEnable() {
    ui::Canvas::OnEnable();
    ShowSoftKeyboard();
}

void UsernamePopup::OnDisable() {
    ui::Canvas::OnDisable();
    HideSoftKeyboard();
}

void UsernamePopup::Update(float dT) {
    SoftKeyboardEditText(usernameBuffer, MAX_USERNAME_SIZE);
    usernameTxt->SetText(usernameBuffer);
}

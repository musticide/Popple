#include "PauseGameCanvas.h"
#include "SceneManager.h"
#include "raylib.h"
#include "uiElement.h"
#include "uiText.h"

PauseGameCanvas::PauseGameCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    darkBgImg = CreateElement<ui::Image>(
        true, "textures/Empty.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);
    darkBgImg->tint = Color{ 0, 0, 0, 200 };

    msgTxt = CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 737, 1080, 200 }, ui::STRETCH_W | ui::FIXED_H);
    msgTxt->hAlign = ui::ALIGN_CENTER;
    msgTxt->vAlign = ui::ALIGN_MIDDLE;
    msgTxt->fontSize = 125;
    msgTxt->SetText("PAUSED");

    resumeBtn                    = CreateElement<ui::Button>(true,
        "textures/EndGameAtlas.png",
        Rectangle{ 161, 1068, 758, 325 },
        "Resume",
        ui::ACE_BOLD_ITALIC,
        ui::STRETCH_W | ui::FIXED_H,
        true);
    resumeBtn->nPatchInfo.source = { 4, 4, 345, 325 };
    resumeBtn->nPatchInfo.layout = NPATCH_NINE_PATCH;
    resumeBtn->nPatchInfo.left   = 135;
    resumeBtn->nPatchInfo.right  = 135;
    resumeBtn->nPatchInfo.top    = 75;
    resumeBtn->nPatchInfo.bottom = 180;
    resumeBtn->fontSize          = 100;
    resumeBtn->fontOffset.y      = 57;

    exitBtn = CreateElement<ui::Button>(
        true, "textures/EndGameAtlas.png", Rectangle{ 161, 1394, 758, 325 }, "Exit", ui::ACE_BOLD_ITALIC, ui::STRETCH_W | ui::FIXED_H, true);
    exitBtn->nPatchInfo.source = { 353, 4, 345, 325 };
    exitBtn->nPatchInfo.layout = NPATCH_NINE_PATCH;
    exitBtn->nPatchInfo.left   = 135;
    exitBtn->nPatchInfo.right  = 135;
    exitBtn->nPatchInfo.top    = 75;
    exitBtn->nPatchInfo.bottom = 180;
    exitBtn->fontSize          = 100;
    exitBtn->fontOffset.y      = 57;

    exitBtn->onClick.connect([]() {
        SceneManager::Get().ActivateScene(SceneType::HOME);
        SceneManager::Get().DestroyScene(SceneType::GAMEPLAY);
    });
}

PauseGameCanvas::~PauseGameCanvas() {
}

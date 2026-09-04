#include "LoadingCanvas.h"
#include "SceneManager.h"
#include "raylib.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiText.h"
#include <string>

LoadingCanvas::LoadingCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);
    loadingPercentTxt =
        CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 1224, 1080, 200 }, ui::STRETCH_W | ui::FIXED_H);
    loadingPercentTxt->hAlign   = ui::ALIGN_CENTER;
    loadingPercentTxt->vAlign   = ui::ALIGN_MIDDLE;
    loadingPercentTxt->fontSize = 85;
    loadingPercentTxt->SetText("loading");
}

LoadingCanvas::~LoadingCanvas() {
}
void LoadingCanvas::Update(float dT) {
    loadingPercentTxt->SetText(
        "Loading " + std::to_string(static_cast<int>(SceneManager::Get().loadingProgress * 100)) + "%");
}

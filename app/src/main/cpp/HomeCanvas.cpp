#include "HomeCanvas.h"
#include "Globals.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiElement.h"

HomeCanvas::HomeCanvas() {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_H | ui::STRETCH_V);

    startButton = CreateElement<ui::Button>(
        true, "textures/StartButton.png", Rectangle{ 223, 981, 616, 279 }, ui::FIXED_H | ui::FIXED_V);
    startButton->onClick.connect(Globals::StartGame);

}

HomeCanvas::~HomeCanvas() {
}

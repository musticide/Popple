#include "HomeCanvas.h"
#include "Globals.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiElement.h"
#include "uiText.h"
#include <cstdlib>
#include <string>

HomeCanvas::HomeCanvas() {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_H | ui::STRETCH_V);

    {
        int size = 0;

        void* data = ReadFromAppStorage("highscore.dat", &size);
        if (data && size == sizeof(int)) {
            memcpy(&Globals::highScore, data, sizeof(int));
            LOGI("HighScore: %d", Globals::highScore);
        } else {
            LOGW("Could not find High Score");
        }
        free(data);
    }
    {
        int size = 0;

        void* data = ReadFromAppStorage("highest_time.dat", &size);
        if (data && size == sizeof(float)) {
            memcpy(&Globals::highestTime, data, sizeof(float));
            LOGI("HighestTime: %f", Globals::highestTime);
        } else {
            LOGW("Could not find Highest time");
        }
        free(data);
    }

    highScoreTxt = CreateElement<ui::Text>(true, Rectangle{ 0, 540, 1080, 100 }, ui::FIXED_H | ui::FIXED_V);
    highScoreTxt->hAlign = ui::ALIGN_CENTER;
    highScoreTxt->vAlign = ui::ALIGN_MIDDLE;
    highScoreTxt->SetText("HighScore: " + std::to_string(Globals::highScore));

    highestTimeTxt= CreateElement<ui::Text>(true, Rectangle{ 0, 680, 1080, 100 }, ui::FIXED_H | ui::FIXED_V);
    highestTimeTxt->hAlign = ui::ALIGN_CENTER;
    highestTimeTxt->vAlign = ui::ALIGN_MIDDLE;
    highestTimeTxt->SetText(TextFormat("Highest Time: %.1f", Globals::highestTime));

    startButton = CreateElement<ui::Button>(
        true, "textures/StartButton.png", Rectangle{ 223, 981, 616, 279 }, ui::FIXED_H | ui::FIXED_V);
    startButton->onClick.connect(Globals::StartGame);
}

HomeCanvas::~HomeCanvas() {
}
void HomeCanvas::OnEnable() {
    highScoreTxt->SetText("HighScore: " + std::to_string(Globals::highScore));
    highestTimeTxt->SetText(TextFormat("Highest Time: %.1f", Globals::highestTime));
}

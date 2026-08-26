#include "EndGameCanvas.h"
#include "GameManager.h"
#include "Globals.h"
#include "LevelConfig.h"
#include "Log.h"
#include "Scene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiImage.h"
#include "uiText.h"
#include <string>

EndGameCanvas::EndGameCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    LOGI("End Game canvas Contructor start");
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);

    starBgGradient = CreateElement<ui::Image>(
        true, "textures/EndGameAtlas.png", Rectangle{ 128, 358, 824, 894 }, ui::STRETCH_W | ui::FIXED_H, true);
    starBgGradient->nPatchInfo.source = { 4, 341, 357, 679 };
    starBgGradient->nPatchInfo.layout = NPATCH_NINE_PATCH;
    starBgGradient->nPatchInfo.left   = 128;
    starBgGradient->nPatchInfo.right  = 128;
    starBgGradient->nPatchInfo.top    = 148;
    starBgGradient->nPatchInfo.bottom = 463;

    starImages[0] = CreateElement<ui::Image>(
        true, "textures/EndGameAtlas.png", Rectangle{ 289, 783, 168, 168 }, ui::FIXED_W | ui::FIXED_H);
    starImages[1] = CreateElement<ui::Image>(
        true, "textures/EndGameAtlas.png", Rectangle{ 457, 731, 168, 168 }, ui::FIXED_W | ui::FIXED_H);
    starImages[2] = CreateElement<ui::Image>(
        true, "textures/EndGameAtlas.png", Rectangle{ 625, 783, 168, 168 }, ui::FIXED_W | ui::FIXED_H);

    starImages[0]->nPatchInfo.source = starOnOffRect[0];
    starImages[1]->nPatchInfo.source = starOnOffRect[0];
    starImages[2]->nPatchInfo.source = starOnOffRect[0];

    starImages[0]->SetScale(1.25);
    starImages[1]->SetScale(1.25);
    starImages[2]->SetScale(1.25);

    nextLevelButton = CreateElement<ui::Button>(true,
        "textures/EndGameAtlas.png",
        Rectangle{ 161, 1378, 758, 325 },
        "Next Level",
        ui::ACE_BOLD_ITALIC,
        ui::STRETCH_W | ui::FIXED_H,
        true);

    nextLevelButton->nPatchInfo.source = { 4, 4, 345, 325 };
    nextLevelButton->nPatchInfo.layout = NPATCH_NINE_PATCH;
    nextLevelButton->nPatchInfo.left   = 135;
    nextLevelButton->nPatchInfo.right  = 135;
    nextLevelButton->nPatchInfo.top    = 75;
    nextLevelButton->nPatchInfo.bottom = 180;
    nextLevelButton->fontSize          = 100;
    nextLevelButton->fontOffset.y      = 57;
    // nextLevelButton->onClick.connect();


    retryLevelButton                    = CreateElement<ui::Button>(true,
        "textures/EndGameAtlas.png",
        Rectangle{ 161, 1641, 758, 325 },
        "Retry Level",
        ui::ACE_BOLD_ITALIC,
        ui::STRETCH_W | ui::FIXED_H,
        true);
    retryLevelButton->nPatchInfo.source = { 4, 4, 345, 325 };
    retryLevelButton->nPatchInfo.layout = NPATCH_NINE_PATCH;
    retryLevelButton->nPatchInfo.left   = 135;
    retryLevelButton->nPatchInfo.right  = 135;
    retryLevelButton->nPatchInfo.top    = 75;
    retryLevelButton->nPatchInfo.bottom = 180;
    retryLevelButton->fontOffset.y      = 57;
    retryLevelButton->fontSize          = 100;


    exitToMainButton = CreateElement<ui::Button>(
        true, "textures/EndGameAtlas.png", Rectangle{ 161, 1904, 758, 325 }, "Exit", ui::ACE_BOLD_ITALIC, ui::STRETCH_W | ui::FIXED_H, true);
    exitToMainButton->nPatchInfo.source = { 353, 4, 345, 325 };
    exitToMainButton->nPatchInfo.layout = NPATCH_NINE_PATCH;
    exitToMainButton->nPatchInfo.left   = 135;
    exitToMainButton->nPatchInfo.right  = 135;
    exitToMainButton->nPatchInfo.top    = 75;
    exitToMainButton->nPatchInfo.bottom = 180;
    exitToMainButton->fontSize          = 100;
    exitToMainButton->fontOffset.y      = 57;

    exitToMainButton->onClick.connect([]() {
        SceneManager::Get().ActivateScene(SceneType::HOME);
        SceneManager::Get().DestroyScene(SceneType::GAMEPLAY);
        // SceneManager::Get().DeactivateScene(SceneType::GAMEPLAY);
    });

    resultText = CreateElement<ui::Text>(
        true, ui::ROUNDED_MPLUS_EXTRABOLD, Rectangle{ 0, 454, 1080, 100 }, ui::FIXED_H | ui::FIXED_W);
    resultText->fontSize = 140;
    resultText->hAlign   = ui::ALIGN_CENTER;
    resultText->SetText("COMPLETE!");

    scoreText = CreateElement<ui::Text>(true, ui::ACE_BOLD, Rectangle{ 0, 1057, 1080, 100 }, ui::FIXED_H | ui::FIXED_W);
    scoreText->fontSize = 95;
    scoreText->hAlign   = ui::ALIGN_CENTER;
    scoreText->SetText("Score:XXX");
    LOGI("End Game canvas Contructor End");
}
EndGameCanvas::~EndGameCanvas() {
}

void EndGameCanvas::Start() {
    ui::Canvas::Start();
    Initialise();
}

void EndGameCanvas::OnEnable() {
    Initialise();
}

void EndGameCanvas::Initialise() {
    LOGI("End Game Canvas Enabled");
    using namespace Globals;
    ui::Canvas::OnEnable();

    scoreText->SetText("Score: " + std::to_string(GameResults.score));

    if (GameResults.gameWon) {
        resultText->SetText("LEVEL\nCOMPLETE!");
    } else {
        resultText->SetText("LEVEL\nFAILED");
    }

    for (size_t i = 0; i < starImages.size(); i++) {
        starImages[i]->nPatchInfo.source = starOnOffRect[i < GameResults.levelRating];
    }

    nextLevelButton->onClick.disconnectAll();
    retryLevelButton->onClick.disconnectAll();

    nextLevelButton->onClick.connect([]() {
        LOGI("Starting Level %d", GameResults.levelPlayed + 1);

        LevelConfig config = GetLevelConfig(GameResults.levelPlayed + 1);
        GameManager::Get().RestartGame(config);
    });

    retryLevelButton->onClick.connect([]() {
        LOGI("Retrying Level %d", GameResults.levelPlayed);
        LevelConfig config = GetLevelConfig(GameResults.levelPlayed);
        GameManager::Get().RestartGame(config);
    });
}

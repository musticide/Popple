#include "LevelMenuCanvas.h"
#include "GameManager.h"
#include "GameplayScene.h"
#include "Globals.h"
#include "LevelConfig.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "RemoteConfig.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Transitions.h"
#include "Tween.h"
#include "TweenManager.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"
#include "uiElement.h"
#include "uiText.h"
#include <cstdlib>
#include <string>
#include <type_traits>


LevelMenuCanvas::LevelMenuCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);


    // Assign the highest Level config + 1
    levelParams = GetLevelParams(PlayerProfile.highestLevelCleared.value + 1);

    for (size_t i = 0; i < MAX_LEVEL_GROUPS; i++) {
        levelButtonsArray[i] = CreateElement<LevelButtonsArray>(true,
            (Rectangle){ 0, 0, (float)Globals::baseScreenHeight, (float)Globals::baseScreenHeight },
            ui::STRETCH_W | ui::STRETCH_H,
            i + 1,
            levelParams);
    }

    activeLevelGroupIndex = std::floor((float)levelParams.levelNumber / MAX_LEVEL_GROUPS);
    levelButtonsArray[activeLevelGroupIndex]->SetActive(true);

    playBtn                    = CreateElement<ui::Button>(true,
        "textures/LevelsMenuAtlas.png",
        Rectangle{ 191, 1732, 697, 350 },
        "PLAY",
        ui::ACE_BOLD_ITALIC,
        ui::STRETCH_W | ui::FIXED_H,
        true);
    playBtn->nPatchInfo.source = { 5, 694, 347, 325 };
    playBtn->nPatchInfo.layout = NPATCH_NINE_PATCH;
    playBtn->nPatchInfo.left   = 130;
    playBtn->nPatchInfo.right  = 130;
    playBtn->nPatchInfo.top    = 70;
    playBtn->nPatchInfo.bottom = 170;
    playBtn->fontOffset.y      = 62;
    playBtn->fontSize          = 120;
    playBtn->onClick.connect([this]() {
        // Change scenemanager to activate scene by reference
        // Or register scene will create a scene inside the scenemanager
        // always only 1 scene of a type will exist
        SceneManager::Get().RegisterScene<GameplayScene>(SceneType::GAMEPLAY, false, levelParams);
        SceneManager::Get().ActivateScene(SceneType::GAMEPLAY);
        SceneManager::Get().DeactivateScene(SceneType::HOME);
    });

    nextBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 784, 1456, 200, 200 }, ui::FIXED_W | ui::FIXED_H);
    nextBtn->nPatchInfo.source = { 535, 5, 200, 200 };
    nextBtn->onClick.connect([this]() {
        activeLevelGroupIndex = (activeLevelGroupIndex + 1) % MAX_LEVEL_GROUPS;
        Transition::SlideInLevelBtnsFromRight(levelButtonsArray[activeLevelGroupIndex].get());
        Transition::SlideOutLevelBtnsToLeft(levelButtonsArray[activeLevelGroupIndex - 1].get());
        Refresh();
    });

    prevBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 109, 1456, 200, 200 }, ui::FIXED_W | ui::FIXED_H);
    prevBtn->nPatchInfo.source = { 535, 5, -200, 200 };
    prevBtn->onClick.connect([this]() {
        activeLevelGroupIndex = (activeLevelGroupIndex - 1) % MAX_LEVEL_GROUPS;
        Transition::SlideInLevelBtnsFromLeft(levelButtonsArray[activeLevelGroupIndex].get());
        Transition::SlideOutLevelBtnsToRight(levelButtonsArray[activeLevelGroupIndex + 1].get());

        Refresh();
    });

    backBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 53, 56, 128, 128 }, ui::FIXED_W | ui::FIXED_H);
    backBtn->SetAnchor({ 0.f, 0.f });
    backBtn->nPatchInfo.source = { 740, 5, 128, 128 };
}

LevelMenuCanvas::~LevelMenuCanvas() {
}
void LevelMenuCanvas::Start() {
    ui::Canvas::Start();
    for (size_t i = 0; i < MAX_LEVEL_GROUPS; i++) {
        levelButtonsArray[i]->SetActive(i == activeLevelGroupIndex);
    }
    Refresh();
}

void LevelMenuCanvas::OnEnable() {
    ui::Canvas::OnEnable();
    Refresh();
    levelButtonsArray[activeLevelGroupIndex]->Refresh();
}

void LevelMenuCanvas::OnDisable() {
    ui::Canvas::OnDisable();
}

void LevelMenuCanvas::Refresh() {
    prevBtn->clickable = activeLevelGroupIndex != 0;
    prevBtn->tint      = activeLevelGroupIndex != 0 ? WHITE : LIGHTGRAY;
    nextBtn->clickable = activeLevelGroupIndex < MAX_LEVEL_GROUPS - 1;
    nextBtn->tint      = activeLevelGroupIndex < MAX_LEVEL_GROUPS - 1 ? WHITE : LIGHTGRAY;
    levelParams = GetLevelParams(PlayerProfile.highestLevelCleared.value + 1);
}

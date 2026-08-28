#include "HomeScene.h"
#include "Globals.h"
#include "PlayerProfile.h"
#include "Scene.h"
#include "raymob.h"
#include "uiCanvas.h"
#include <memory>

HomeScene::HomeScene()
: Scene(SceneType::HOME) {
    touchFeedback   = CreateEntity<TouchFeedback>(true);
    homeCanvas      = CreateEntity<HomeCanvas>(true);
    levelMenuCanvas = CreateEntity<LevelMenuCanvas>(false);
    homeCanvas->startButton->onClick.connect([this]() {
        if (PlayerProfile.username.value.empty()) {
            homeCanvas->usernamePopup->SetActive(true);
        } else {
            levelMenuCanvas->SetActive(true);
            homeCanvas->SetActive(false);
        }
    });
    levelMenuCanvas->backBtn->onClick.connect([this]() {
        homeCanvas->SetActive(true);
        levelMenuCanvas->SetActive(false);
    });
}

HomeScene::~HomeScene() {
}

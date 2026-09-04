#include "HomeScene.h"
#include "Globals.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "Scene.h"
#include "Transitions.h"
#include "raymob.h"
#include "uiCanvas.h"
#include <memory>

HomeScene::HomeScene()
: Scene(SceneType::HOME) {
    homeCanvas      = CreateEntity<HomeCanvas>(true);
    levelMenuCanvas = CreateEntity<LevelMenuCanvas>(false);
    homeCanvas->classicModeBtn->onClick.connect([this]() {
        if (PlayerProfile.username.value.empty()) {
            usernamePopup->SetActive(true);
        } else {
            Transition::SlideInCanvas(homeCanvas.get(), levelMenuCanvas.get());
        }
    });
    levelMenuCanvas->backBtn->onClick.connect([this]() {
        Transition::FadeOutTopCanvas(levelMenuCanvas.get(), homeCanvas.get());
    });

    usernamePopup = CreateEntity<UsernamePopup>(false);
    isLoaded = true;
}

HomeScene::~HomeScene() {
}
void HomeScene::Load() {
}


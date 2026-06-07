#include "HomeScene.h"
#include "Globals.h"
#include "Scene.h"
#include "uiCanvas.h"
#include <memory>

HomeScene::HomeScene() 
    : Scene(SceneType::HOME) 
{
    touchFeedback = CreateEntity<TouchFeedback>(true);
    homeCanvas = CreateEntity<HomeCanvas>(true);
    levelMenuCanvas = CreateEntity<LevelMenuCanvas>(false);
    homeCanvas->startButton->onClick.connect([this](){
        levelMenuCanvas->SetActive(true);
        homeCanvas->SetActive(false);
    });
    levelMenuCanvas->backBtn->onClick.connect([this](){
        homeCanvas->SetActive(true);
        levelMenuCanvas->SetActive(false);
    });
}

HomeScene::~HomeScene() {
}

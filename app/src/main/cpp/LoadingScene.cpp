#include "LoadingScene.h"
#include "Log.h"
#include "Scene.h"

LoadingScene::LoadingScene()
: Scene(SceneType::LOADING){

    loadingCanvas = CreateEntity<LoadingCanvas>(true);

    isLoaded = true;
    LOGI("Loading Scene Constructed");
}

LoadingScene::~LoadingScene() {
}

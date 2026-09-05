#include "LoadingScene.h"
#include "Log.h"
#include "Scene.h"

LoadingScene::LoadingScene()
: Scene(SceneType::LOADING){

    loadingCanvas = CreateEntity<LoadingCanvas>(true);

    isLoaded = true;
    m_Name = "Loading";
}

LoadingScene::~LoadingScene() {
}

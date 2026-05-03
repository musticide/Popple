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
}

HomeScene::~HomeScene() {
}

#include "TestScene.h"

TestScene::TestScene()
: Scene(SceneType::TEST) {
    m_Name     = "TestScene";
    testEntity = CreateEntity<TestEntity>(true);
    isLoaded   = true;
}

TestScene::~TestScene() {
}

void TestScene::Load() {
}

#include "GameplayScene.h"
#include "ResourceManager.h"

GameplayScene::GameplayScene() 
:Scene("GameplayScene")
{
    m_SpatialGrid = CreateEntity<SpatialGrid>();
    m_BubbleManager = CreateEntity<BubbleManager>();

    m_Tower = CreateEntity<Tower>();
    m_Tower->SetActive(true);

    m_Background = CreateEntity<StaticMesh>("models/Quad.glb");
    m_Background->GetModel().materials[0].shader = *ResourceManager::GetShader(0, "shaders/background.fs");
    m_Background->GetModel().materials[0].maps[0].texture
        = *ResourceManager::GetTexture("textures/T_CheckerBackground.png");
    m_Background->position = { 0.f, -20.f, 0.f };
    m_Background->scale = Vector3Scale(Vector3One(), 2.f);
    m_Background->SetActive(true);

    m_GameUI = CreateEntity<GameUI>();
    m_GameUI->SetActive(true);
}

GameplayScene::~GameplayScene() {
}

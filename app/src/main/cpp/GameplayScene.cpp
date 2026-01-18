#include "GameplayScene.h"
#include "DrawableEntity.h"
#include "Log.h"
#include "ResourceManager.h"

GameplayScene::GameplayScene() 
:Scene("GameplayScene")
{
    m_GameManager = CreateEntity<GameManager>(true);
    m_EffectManager = CreateEntity<EffectManager>(true);
    m_SpatialGrid = CreateEntity<SpatialGrid>(true);
    m_BubbleManager = CreateEntity<BubbleManager>(true);

    m_Tower = CreateEntity<Tower>(true);

    m_Background = CreateEntity<StaticMesh>(true, "models/Quad.glb", RenderQueue::SKY);
    m_Background->GetModel().materials[0].shader = *ResourceManager::GetShader(0, "shaders/background.fs");
    m_Background->GetModel().materials[0].maps[0].texture
        = *ResourceManager::GetTexture("textures/T_CheckerBackground.png");
    m_Background->position = { 0.f, -20.f, 0.f };
    m_Background->scale = Vector3Scale(Vector3One(), 2.f);

    m_GameUI = CreateEntity<GameUI>(true);
    // m_GameUI->SetActive(true);
}

GameplayScene::~GameplayScene() {
}

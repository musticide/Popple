#include "GameplayScene.h"
#include "DrawableEntity.h"
#include "GameCanvas.h"
#include "GameManager.h"
#include "LevelConfig.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"


GameplayScene::GameplayScene(LevelConfig config) : Scene(SceneType::GAMEPLAY) {
    m_GameManager   = CreateEntity<GameManager>(true, config);

    m_Tower = CreateEntity<Tower>(true);

    m_Background = CreateEntity<StaticMesh>(true, "models/Quad.glb", RenderQueue::SKY);
    m_Background->GetModel().materials[0].shader = *ResourceManager::GetShader(0, "shaders/background.frag");
    m_Background->GetModel().materials[0].maps[0].texture =
        *ResourceManager::GetTexture("textures/T_GameBG.png");
    m_Background->position = { 0.f, -20.f, 0.f };
    m_Background->scale    = Vector3Scale(Vector3One(), 2.f);

    m_GameCanvas = CreateEntity<GameCanvas>(true);
}

GameplayScene::~GameplayScene() {
}

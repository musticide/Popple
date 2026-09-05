#include "GameplayScene.h"
#include "DrawableEntity.h"
#include "GameCanvas.h"
#include "GameManager.h"
#include "LevelConfig.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"


GameplayScene::GameplayScene(LevelParams config)
: Scene(SceneType::GAMEPLAY)
, levelParams(config) {
    m_TotalSteps = 6;
    m_Name = "Gameplay";
}

GameplayScene::~GameplayScene() {
}
void GameplayScene::Load() {
    switch (m_LoadStep) {
        case 0:
            m_Tower = CreateEntity<Tower>(true);
            break;

        case 1:
            m_Background = CreateEntity<StaticMesh>(true, "models/Quad.glb", RenderQueue::SKY);
            m_Background->GetModel().materials[0].shader = *ResourceManager::GetShader(0, "shaders/background.frag");
            m_Background->GetModel().materials[0].maps[0].texture =
                *ResourceManager::GetTexture("textures/T_GameBG.png");
            m_Background->position = { 0.f, -20.f, 0.f };
            m_Background->scale    = Vector3Scale(Vector3One(), 2.f);
            break;

        case 2:
            m_GameCanvas = CreateEntity<GameCanvas>(true, levelParams);
            break;

        case 3:
            pauseGameCanvas = CreateEntity<PauseGameCanvas>(false);
            break;

        case 4:
            endGameCanvas = CreateEntity<EndGameCanvas>(false, levelParams);
            break;

        case 5:
            m_GameManager = CreateEntity<GameManager>(
                true, levelParams, m_GameCanvas.get(), pauseGameCanvas.get(), endGameCanvas.get());
            break;
    }
    m_LoadStep++;

    if (m_LoadStep >= m_TotalSteps) {
        isLoaded = true;
        SceneManager::Get().DeactivateScene(SceneType::LOADING);
    }
}

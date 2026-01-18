#pragma once

#include "GameManager.h"
#include "GameUI.h"
#include "Scene.h"
#include "StaticMesh.h"
#include "Tower.h"
#include "bubbleManager.h"
#include "spatialGrid.h"
#include "EffectManager.h"
#include <memory>

class GameplayScene : public Scene{
public:
    GameplayScene();
    ~GameplayScene();

private:

    //Entities
    std::unique_ptr<GameManager> m_GameManager;
    std::unique_ptr<EffectManager> m_EffectManager;
    std::unique_ptr<BubbleManager> m_BubbleManager;
    std::unique_ptr<Tower> m_Tower;
    std::unique_ptr<StaticMesh> m_Background;
    std::unique_ptr<SpatialGrid> m_SpatialGrid;
    std::unique_ptr<GameUI> m_GameUI;

};


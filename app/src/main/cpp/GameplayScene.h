#pragma once

#include "EffectManager.h"
#include "EndGameCanvas.h"
#include "GameCanvas.h"
#include "GameManager.h"
#include "LevelConfig.h"
#include "PauseGameCanvas.h"
#include "Scene.h"
#include "StaticMesh.h"
#include "TouchFeedback.h"
#include "Tower.h"
#include "bubbleManager.h"
#include "spatialGrid.h"
#include "uiCanvas.h"
#include <memory>
#include <string>

class GameplayScene : public Scene {
  public:
    GameplayScene(LevelParams config);
    ~GameplayScene();

  private:
    // Entities
    // NOTE: Destruction runs in reverse declaration order. GameManager holds raw
    // pointers to the canvases below, so declare it LAST so it is destroyed FIRST.
    std::unique_ptr<Tower> m_Tower               = nullptr;
    std::unique_ptr<StaticMesh> m_Background     = nullptr;
    std::unique_ptr<GameCanvas> m_GameCanvas     = nullptr;
    std::unique_ptr<PauseGameCanvas> pauseGameCanvas  = nullptr;
    std::unique_ptr<EndGameCanvas> endGameCanvas = nullptr;
    std::unique_ptr<GameManager> m_GameManager   = nullptr;
    // std::unique_ptr<EffectManager> m_EffectManager;
    // std::unique_ptr<BubbleManager> m_BubbleManager;
    // std::unique_ptr<SpatialGrid> m_SpatialGrid;
};

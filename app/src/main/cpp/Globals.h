#pragma once
#include "gameData.h"
#include "score.h"
#include "SceneManager.h"
#include "GameStateManager.h"

namespace Globals {
    void Reset()
    {
        GameData::Reset();
        // BubbleManager::Reset();
        Score::ResetHealth();
        Score::ResetScore();
    }

    void StartGame()
    {
        // GameStateManager::ChangeGameState(GameState::GAMEPLAY);
        SceneManager::Get().ActivateScene("GameplayScene");
        SceneManager::Get().DeactivateScene("HomeScene");

        LOGI("Gameplay scene activated");

        // TODO: Call reset on scene load
        //  Reset();
    }
}

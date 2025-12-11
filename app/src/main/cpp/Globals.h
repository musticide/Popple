#pragma once
#include "SceneManager.h"
#include "GameStateManager.h"

namespace Globals {

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

#include "GameStateManager.h"

GameStateManager::GameStateManager() 
: m_GameState(MAIN_MENU)
{
}

GameStateManager::~GameStateManager() {
}

void GameStateManager::ChangeStateInternal(GameState state)
{
    m_GameState = state;
    LOGI("GameState changed to : %d", state);
}

void GameStateManager::Init()
{
    Get().SetActive(true);
    LOGI("Game State Manager Initialized");
}

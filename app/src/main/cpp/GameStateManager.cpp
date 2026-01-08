#include "GameStateManager.h"

GameStateManager::  GameStateManager()
: m_GameState(GameState::MAIN_MENU)
{
}

GameStateManager::~GameStateManager() {
}

void GameStateManager::ChangeStateInternal(GameState state)
{
    m_GameState = state;
    LOGI("GameState changed to : %d", state);
}

void GameStateManager::Start() { 
    Get().SetActive(true);
}


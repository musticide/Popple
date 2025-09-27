#pragma once

#include "Entity.h"
#include "Log.h"

enum GameState { MAIN_MENU, GAMEPLAY, PAUSED, GAME_OVER };

class GameStateManager : public Entity {
private:
    GameStateManager();

    static GameStateManager& Get()
    {
        static GameStateManager instance;
        return instance;
    }

    GameState m_GameState;

    void ChangeStateInternal(GameState state);

public:
    GameStateManager(GameStateManager&&) = delete;
    GameStateManager(const GameStateManager&) = delete;
    GameStateManager& operator=(GameStateManager&&) = delete;
    GameStateManager& operator=(const GameStateManager&) = delete;
    ~GameStateManager();

    static void Init();

    static GameState GetState() { return Get().m_GameState; }

    static void ChangeGameState(GameState state) { Get().ChangeStateInternal(state); }
};

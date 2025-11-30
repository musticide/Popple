#pragma once

#include "Entity.h"
#include "Log.h"
#include "Singleton.h"

enum class GameState { MAIN_MENU, GAMEPLAY, PAUSED, GAME_OVER };

class GameStateManager : public Entity, public Singleton<GameStateManager>{
private:
    GameState m_GameState;

    void ChangeStateInternal(GameState state);

public:
    GameStateManager();
    ~GameStateManager();

    void Start() override;

    static GameState GetState() { return Get().m_GameState; }

    static void ChangeGameState(GameState state) { Get().ChangeStateInternal(state); }
};

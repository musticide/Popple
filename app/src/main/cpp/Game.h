#pragma once

#include <memory>
#include "input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "gameData.h"
#include "score.h"
#include "raylib.h"
#include "HomeScene.h"
#include "GameplayScene.h"

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();
    void Shutdown();

private:
    inline static Game* s_Instance = nullptr;

    std::unique_ptr<Input> m_InputManager;
    std::unique_ptr<ResourceManager> m_ResourceManager;
    std::unique_ptr<GameData> m_GameData;
    std::unique_ptr<SceneManager> m_SceneManager;
    std::unique_ptr<Score> m_Score;

    Camera3D mainCamera3D;
    Camera2D uiCamera;

    std::unique_ptr<HomeScene> m_HomeScene;
    std::unique_ptr<GameplayScene> m_GameplayScene;

    int screenWidth, screenHeight;
};

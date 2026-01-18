#pragma once

#include <memory>
#include "input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "raylib.h"
#include "HomeScene.h"
#include "GameplayScene.h"
#include "Renderer.h"

class Game {
public:
    Game();
    ~Game();

    void Init();
    void Run();
    void Shutdown();

    static Game& Get()
    {
        assert(s_Instance && "Game not initialized");
        return *s_Instance;
    }

    Camera3D mainCamera3D;
    Camera2D uiCamera;

    int screenWidth, screenHeight;

private:
    inline static Game* s_Instance = nullptr;

    std::unique_ptr<Input> m_InputManager;
    std::unique_ptr<ResourceManager> m_ResourceManager;
    std::unique_ptr<SceneManager> m_SceneManager;
    std::unique_ptr<Renderer> m_Renderer;

    std::unique_ptr<HomeScene> m_HomeScene;
    std::unique_ptr<GameplayScene> m_GameplayScene;
};

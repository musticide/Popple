#pragma once

#include <memory>
// #include "editor/NetworkReceiver.h"
#include "input.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "raylib.h"
#include "HomeScene.h"
#include "GameplayScene.h"
#include "editor/EditorScene.h"
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
    std::unique_ptr<EditorScene> m_EditorScene;

    // NetworkReceiver* netReceiver;

    RenderTexture2D renderTarget;
    Shader bloomShader;
};

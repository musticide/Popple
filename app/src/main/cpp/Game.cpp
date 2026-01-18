#include "Game.h"
#include "Globals.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "raylib.h"
#include <cassert>
#include <memory>

Game::Game()
{
    assert(!s_Instance && "Game already exists");
    s_Instance = this;
}

Game::~Game() { }
void Game::Init()
{
    InitWindow(0, 0, "Popple");

    screenWidth = Globals::screenWidth = GetScreenWidth();
    screenHeight = Globals::screenHeight = GetScreenHeight();

    uiCamera          = { 0 };
    uiCamera.target   = (Vector2) { 0.0f, 0.0f };
    uiCamera.offset   = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };
    uiCamera.rotation = 0.0f;
    uiCamera.zoom     = 1.0f;

    mainCamera3D          = { 0 };
    mainCamera3D.position = (Vector3) { 0.0f, 20.0f, 0.0f }; // Camera position
    mainCamera3D.target   = (Vector3) { 0.0f, 0.0f, 0.0f }; // Looking at the origin
    mainCamera3D.up
        = (Vector3) { 0.0f, 0.0f, 1.0f }; // "Up" points toward +Z since we're looking down Y
    mainCamera3D.fovy       = 80.0f;
    mainCamera3D.projection = CAMERA_ORTHOGRAPHIC;

    // Managers/Systems
    m_InputManager    = std::make_unique<Input>(&mainCamera3D, &uiCamera);
    m_ResourceManager = std::make_unique<ResourceManager>();
    m_SceneManager    = std::make_unique<SceneManager>();
    m_Renderer        = std::make_unique<Renderer>(mainCamera3D, uiCamera);

    m_HomeScene     = SceneManager::Get().RegisterScene<HomeScene>();
    m_GameplayScene = SceneManager::Get().RegisterScene<GameplayScene>();

    m_HomeScene->SetActive(true);

    SetTargetFPS(60);
}

void Game::Run()
{
    while (!WindowShouldClose()) {
        SceneManager::Get().StartScenes();
        SceneManager::Get().UpdateScenes(GetFrameTime());

        Renderer::Get().Render();

    }
}

void Game::Shutdown()
{
    Renderer::Get().Cleanup();
    CloseWindow();
}

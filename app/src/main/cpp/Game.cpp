#include "Game.h"
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

    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    uiCamera = { 0 };
    uiCamera.target = (Vector2) { 0.0f, 0.0f };
    uiCamera.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };
    uiCamera.rotation = 0.0f;
    uiCamera.zoom = 1.0f;

    mainCamera3D = { 0 };
    mainCamera3D.position = (Vector3) { 0.0f, 20.0f, 0.0f }; // Camera position
    mainCamera3D.target = (Vector3) { 0.0f, 0.0f, 0.0f }; // Looking at the origin
    mainCamera3D.up = (Vector3) { 0.0f, 0.0f, -1.0f }; // "Up" points toward -Z since we're looking down Y
    mainCamera3D.fovy = 80.0f;
    mainCamera3D.projection = CAMERA_ORTHOGRAPHIC;

    m_InputManager = std::make_unique<Input>(&mainCamera3D, &uiCamera);
    m_ResourceManager = std::make_unique<ResourceManager>();
    m_GameData = std::make_unique<GameData>();
    m_SceneManager = std::make_unique<SceneManager>();
    m_Score = std::make_unique<Score>();

    m_HomeScene = SceneManager::Get().RegisterScene<HomeScene>();
    m_GameplayScene = SceneManager::Get().RegisterScene<GameplayScene>();

    m_HomeScene->SetActive(true);

    SetTargetFPS(60);
}

void Game::Run() {

    while (!WindowShouldClose()) {
        SceneManager::Get().StartScenes();
        SceneManager::Get().UpdateScenes(GetFrameTime());

        BeginDrawing(); //---------------------------------------------------||

        ClearBackground(BLACK);
        BeginMode3D(mainCamera3D);
        SceneManager::Get().DrawScenes();
        EndMode3D();

        BeginMode2D(uiCamera); //-----------------------------------||
        SceneManager::Get().DrawUI();
        EndMode2D();
        EndDrawing();
    }
}

void Game::Shutdown() { CloseWindow(); }


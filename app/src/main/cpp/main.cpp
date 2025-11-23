#include "Button.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GameStateManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "StaticMesh.h"
#include "Tower.h"
#include "bubbleManager.h"
#include "gameData.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"
#include "spatialGrid.h"
#include <memory>
#include <raymob.h>

#define BUBBLE_COUNT 50

Camera3D camera;
Camera2D camera2D;

int GetPercent(int a, int total) { return (a / total) * 100; }

void Reset()
{
    GameData::Reset();
    BubbleManager::Reset();
    Score::ResetHealth();
    Score::ResetScore();
}

void StartGame()
{
    Reset();
    GameStateManager::ChangeGameState(GameState::GAMEPLAY);
    BubbleManager::Get()->SetActive(true);
}

int main()
{
    InitWindow(0, 0, "Popple");

    const int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();

    camera2D = { 0 };
    camera2D.target = (Vector2) { 0.0f, 0.0f };
    camera2D.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };

    camera2D.rotation = 0.0f;
    camera2D.zoom = 1.0f;

    camera = { 0 };
    camera.position = (Vector3) { 0.0f, 20.0f, 0.0f }; // Camera position
    camera.target = (Vector3) { 0.0f, 0.0f, 0.0f }; // Looking at the origin
    camera.up = (Vector3) { 0.0f, 0.0f, -1.0f }; // "Up" points toward -Z since we're looking down Y
    camera.fovy = 80.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    Input::initialize(camera, camera2D);
    // BubbleManager::Init();
    // GameStateManager::Init();

    std::unique_ptr<Scene> globalScene = std::make_unique<Scene>("Global Scene");
    SceneManager::Get()->AddScene(globalScene.get());

    globalScene->AddEntity(std::make_unique<GameStateManager>());
    GameStateManager::ChangeGameState(GameState::MAIN_MENU);

    SceneManager::Get()->LoadScene(globalScene.get());
    globalScene->SetActive(true);

    std::unique_ptr<Scene> homeScene = std::make_unique<Scene>("Home Scene");
    SceneManager::Get()->AddScene(homeScene.get());

    Button* playButton = homeScene->CreateEntity<Button>("textures/start_button.png", WHITE, (Vector2) { 0, 0 });
    playButton->AddOnClickListener(StartGame);
    playButton->SetActive(true);

    SceneManager::Get()->LoadScene(homeScene.get());
    homeScene->SetActive(true);

    Scene gameplayScene("Gameplay Scene");
    //TODO: spatial grid stuff

    Tower* tower = gameplayScene.CreateEntity<Tower>();
    StaticMesh* background = gameplayScene.CreateEntity<StaticMesh>("models/Quad.glb");
    background->GetModel().materials[0].shader = *ResourceManager::GetShader(0, "shaders/background.fs");
    background->GetModel().materials[0].maps[0].texture
        = *ResourceManager::GetTexture("textures/T_CheckerBackground.png");

    background->position = { 0.f, -20.f, 0.f };
    background->scale = Vector3Scale(Vector3One(), 2.f);


    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        SceneManager::Get()->StartScenes();
        SceneManager::Get()->UpdateScenes(GetFrameTime());

        BeginDrawing(); //---------------------------------------------------||

        ClearBackground(BLACK);
        BeginMode3D(camera);
        SceneManager::Get()->DrawScenes();
        EndMode3D();

        BeginMode2D(camera2D); //-----------------------------------||
        SceneManager::Get()->DrawUI();
        EndMode2D();
        EndDrawing();

    }

    CloseWindow();
}

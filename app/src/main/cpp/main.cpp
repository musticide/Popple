#include "Button.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GameStateManager.h"
#include "Log.h"
#include "StaticMesh.h"
#include "Tower.h"
#include "bubbleManager.h"
#include "gameData.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"
#include "spatialGrid.h"
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
    GameStateManager::ChangeGameState(GAMEPLAY);
    BubbleManager::Get().SetActive(true);
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
    BubbleManager::Init();
    GameStateManager::Init();
    GameStateManager::ChangeGameState(MAIN_MENU);

    Tower tower;
    StaticMesh background("models/Quad.glb");
    background.GetModel().materials[0].shader = LoadShader(0, "shaders/background.fs");
    background.GetModel().materials[0].maps[0].texture = LoadTexture("textures/T_CheckerBackground.png");
    // background.GetModel().materials[0].params[1] = (Vector4){0.f, 0.f, 1.f, 1.f};
    background.position = { 0.f, -20.f, 0.f };
    background.scale = Vector3Scale(Vector3One(), 2.f);

    for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
        EntityManager::GetEntityAt(i)->LoadResources();
        LOGI("Loading resources: %d", GetPercent(i, EntityManager::GetEntityCount()));
    }

    for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
        EntityManager::GetEntityAt(i)->Start();
    }

    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;

    Button playButton("textures/start_button.png", WHITE, { 0, 0 });
    playButton.AddOnClickListener(StartGame);
    playButton.SetActive(true);

    EntityManager::SortEntitiesByRenderMode();

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        switch (GameStateManager::GetState()) {

        case MAIN_MENU:
            background.SetActive(false);
            BeginDrawing(); //---------------------------------------------------||

            ClearBackground(BLACK);

            BeginMode2D(camera2D); //-----------------------------------||

            playButton.Update(GetFrameTime());
            playButton.Draw();

            EndMode2D(); //-------------------------------------------||
            EndDrawing(); //----------------------------------------------------||
            // GameStateManager::ChangeGameState(GAMEPLAY);

            break;

        case GAMEPLAY:
            playButton.SetActive(false);
            background.SetActive(true);

            SpatialGrid::Clear();

            //---DRAWING AND UPDATE LOOP
            // will separate if needed

            GameData::Update(GetFrameTime());

            BeginDrawing(); //---------------------------------------------------||

            ClearBackground(BLACK);

            BeginMode3D(camera); //-----------------------------------||

            // DrawRectanglePro(tower, towerCenter, towerRotation, BEIGE);
            // DrawModelEx(tower, (Vector3){0}, Vector3Zero(), 0, Vector3One(), WHITE);
            // DrawModel(tower, towerPosition, 1.0f, WHITE);

            BeginBlendMode(0);
            for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
                auto e = EntityManager::GetEntityAt(i);
                if (e != NULL && e->IsActive()) {
                    e->Update(GetFrameTime());
                    e->Draw();
                }
            }

            switch (GameData::GetActiveElementalEffect()) {

            case NO_ELEMENTAL_EFFECT:
                break;
            case ELECTRO:
                DrawCircleLinesV({ 0 }, 200, PURPLE);
                break;
            case ANEMO:
                DrawCircleLinesV({ 0 }, 200, GREEN);
                break;
            case ELEMENTAL_EFFECT_COUNT:
                break;
            }

            EndBlendMode();

            EndMode3D(); //-------------------------------------------||

            BeginMode2D(camera2D); //---------------------------------||
            Score::ShowScore();
            Score::ShowHealth();
            GameData::DrawComboCount();
            EndMode2D(); //-------------------------------------------||

            EndDrawing(); //----------------------------------------------------||

            if (Score::GetHealth() <= 0) {
                GameStateManager::ChangeGameState(GAME_OVER);
            }

            break;

        case PAUSED:
        case GAME_OVER:
            background.SetActive(false);
            BeginDrawing(); //---------------------------------------------------||

            ClearBackground(BLACK);

            BeginMode2D(camera2D); //-----------------------------------||
            Score::ShowScore();
            Score::ShowHealth();
            playButton.Update(GetFrameTime());
            playButton.Draw();
            EndMode2D(); //-------------------------------------------||
            EndDrawing(); //----------------------------------------------------||
            break;
        }
    }

    CloseWindow();
}

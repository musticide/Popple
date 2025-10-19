#include "Button.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GameStateManager.h"
#include "Log.h"
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
    camera.position = (Vector3){ 0.0f, 20.0f, 0.0f };    // Camera position
    camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Looking at the origin
    camera.up = (Vector3){ 0.0f, 0.0f, -1.0f };         // "Up" points toward -Z since we're looking down Y
    camera.fovy = 80.0f;
    camera.projection = CAMERA_ORTHOGRAPHIC;

    Input::initialize(camera, camera2D);
    BubbleManager::Init();
    GameStateManager::Init();
    GameStateManager::ChangeGameState(MAIN_MENU);

    // Rectangle tower = { -50.0f, -50.0f, 100.0f, 100.0f };
    Model tower = LoadModel("models/TowerBase.glb");
    // Vector3 towerPosition = (Vector3){17,0,40};
    Vector3 towerPosition = (Vector3){0};

    float towerRotation = 0.0f;
    // Vector3 towerCenter = { tower.x + tower.width / 2, tower.y + tower.height / 2 };

    for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
        EntityManager::GetEntityAt(i)->Start();
    }

    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;

    Button playButton("textures/start_button.png", WHITE, { 0, 0 });
    playButton.AddOnClickListener(StartGame);
    playButton.SetActive(true);

    SetTargetFPS(60);

    while (!WindowShouldClose()) {

        switch (GameStateManager::GetState()) {

        case MAIN_MENU:
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

            SpatialGrid::Clear();

            //---DRAWING AND UPDATE LOOP
            // will separate if needed

            GameData::Update(GetFrameTime());

            BeginDrawing(); //---------------------------------------------------||

            ClearBackground(BLACK);

            BeginMode3D(camera); //-----------------------------------||

            // DrawRectanglePro(tower, towerCenter, towerRotation, BEIGE);
            // DrawModelEx(tower, (Vector3){0}, Vector3Zero(), 0, Vector3One(), WHITE);
            DrawModel(tower, towerPosition, 1.0f, WHITE);
        

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

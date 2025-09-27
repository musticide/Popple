#include "Button.h"
#include "Entity.h"
#include "EntityManager.h"
#include "GameStateManager.h"
#include "Log.h"
#include "bubbleManager.h"
#include "gameData.h"
#include "input.h"
#include "raylib.h"
#include "score.h"
#include "spatialGrid.h"
#include <raymob.h>

#define BUBBLE_COUNT 50

Camera2D camera;

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

    camera = { 0 };
    camera.target = (Vector2) { 0.0f, 0.0f };
    camera.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Input::initialize(camera);
    BubbleManager::Init();
    GameStateManager::Init();
    GameStateManager::ChangeGameState(MAIN_MENU);

    Rectangle tower = { -50.0f, -50.0f, 100.0f, 100.0f };
    float towerRotation = 0.0f;
    Vector2 towerCenter = { tower.x + tower.width / 2, tower.y + tower.height / 2 };

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

            BeginMode2D(camera); //-----------------------------------||

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

            BeginMode2D(camera); //-----------------------------------||

            DrawRectanglePro(tower, towerCenter, towerRotation, BEIGE);

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

            BeginMode2D(camera); //-----------------------------------||
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

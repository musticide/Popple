#include "Entity.h"
#include "EntityManager.h"
#include "Log.h"
#include "bubble.h"
#include "bubbleManager.h"
#include "gameData.h"
#include "input.h"
#include "popple.h"
#include "raylib.h"
#include "score.h"
#include "spatialGrid.h"
#include <raymob.h>

#define BUBBLE_COUNT 50

Camera2D camera;

int main()
{
    InitWindow(0, 0, "Popple");
    SetTargetFPS(60);

    const int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();

    camera = { 0 };
    camera.target = (Vector2) { 0.0f, 0.0f };
    camera.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };

    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Input::initialize(camera);

    Rectangle tower = { -50.0f, -50.0f, 100.0f, 100.0f };
    float towerRotation = 0.0f;
    Vector2 towerCenter = { tower.x + tower.width / 2, tower.y + tower.height / 2 };

    BubbleManager::Init();

    for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
        EntityManager::GetEntityAt(i)->Start();
    }

    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        SpatialGrid::Clear();

        //---DRAWING AND UPDATE LOOP
        // will separate if needed

        GameData::Update(GetFrameTime());

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawRectanglePro(tower, towerCenter, towerRotation, BEIGE);

        for (int i = 0; i < EntityManager::GetEntityCount(); i++) {
            auto e = EntityManager::GetEntityAt(i);
            if (e != NULL && e->IsActive()) {
                e->Update(GetFrameTime());
                e->Draw();
            }
        }

        if (GameData::ElectroShieldActive()) {
            DrawCircleLinesV({ 0 }, 200, PURPLE);
        }

        Score::ShowScore();
        Score::ShowHealth();

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}

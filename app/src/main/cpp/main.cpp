#include "Log.h"
#include "bubble.h"
#include "input.h"
#include "popple.h"
#include "raylib.h"
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

    Bubble* bubbles = new Bubble[BUBBLE_COUNT];

    for (int i = 0; i < BUBBLE_COUNT; i++) {
        bubbles[i].Start();
        bubbles[i].SetActive(false);
    }

    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        spawnTimer += GetFrameTime();

        if (spawnTimer > spawnInterval) {
            spawnInterval = GetRandomValue(20, 5)/10.0f;
            spawnTimer = 0.0f;
            for (int i = 0; i < BUBBLE_COUNT; i++) {
                if (!bubbles[i].IsActive())
                {
                    LOGI("Bubble Spawned at frame: %f", GetTime());
                    bubbles[i].SetActive(true);
                    bubbles[i].Spawn();
                    break;
                }
            }
        }

        SpatialGrid::Clear();

        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode2D(camera);

        DrawRectanglePro(tower, towerCenter, towerRotation, BEIGE);
        for (int i = 0; i < BUBBLE_COUNT; i++) {
            bubbles[i].Update(GetFrameTime());
            bubbles[i].Draw();
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}

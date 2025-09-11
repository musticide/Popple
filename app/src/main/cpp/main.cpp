#include "bubble.h"
#include "raylib.h"
#include <raymob.h>

#define BUBBLE_COUNT 100

int main()
{
    const Color darkGreen = { 20, 160, 133, 255 };

    InitWindow(0, 0, "Popple");
    SetTargetFPS(60);

    const int screenWidth = GetScreenWidth(), screenHeight = GetScreenHeight();

    Camera2D camera = { 0 };
    camera.target = (Vector2) { 0.0f, 0.0f };
    camera.offset = (Vector2) { screenWidth / 2.0f, screenHeight / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    Rectangle tower = { -25.0f, -25.0f, 50.0f, 50.0f };
    float towerRotation = 45.0f;
    Vector2 towerCenter = { tower.x + tower.width / 2, tower.y + tower.height / 2 };

    Bubble* bubbles = new Bubble[BUBBLE_COUNT];

    for (int i = 0; i < BUBBLE_COUNT; i++) {
        bubbles[i].position = (Vector2) { (float)GetRandomValue(-screenWidth, screenWidth),
            (float)GetRandomValue(-screenHeight, screenHeight) };
        bubbles[i].radius = GetRandomValue(40, 35);
        bubbles[i].moveSpeed = (Vector2) { (float)GetRandomValue(1.0f, 3.0f), (float)GetRandomValue(1.0f, 3.0f) };
    }

    SetTargetFPS(60);
    while (!WindowShouldClose()) {

        BeginDrawing();

        ClearBackground(DARKPURPLE);

        BeginMode2D(camera);

        DrawRectanglePro(tower, towerCenter, towerRotation, RAYWHITE);
        for (int i = 0; i < BUBBLE_COUNT; i++) {
            bubbles[i].Update();
            DrawCircleV(bubbles[i].position, bubbles[i].radius, bubbles[i].color);
        }

        EndMode2D();

        EndDrawing();
    }

    CloseWindow();
}

#include <raymob.h>
#include "ball.h"

int main() 
{
    const Color darkGreen = {20, 160, 133, 255};
    Ball ball;

    InitWindow(0, 0, "My first RAYLIB program!");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        ball.Update();

        BeginDrawing();
            ClearBackground(darkGreen);
            ball.Draw();
        EndDrawing();
    }

    CloseWindow();
}

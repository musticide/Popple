#include "Game.h"
#include <raymob.h>

int GetPercent(int a, int total) { return (a / total) * 100; }

int main()
{
    Game game;
    game.Init();
    game.Run();
    game.Shutdown();
}

#include <raymob.h>
#include "Log.h"
#include "Game.h"

int main()
{
    Game game;
    game.Init();
    game.Run();
    game.Shutdown();
}

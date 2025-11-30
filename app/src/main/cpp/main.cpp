#include "Button.h"
#include "Entity.h"
#include "Game.h"
#include "GameStateManager.h"
#include "Log.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "StaticMesh.h"
#include "Tower.h"
// #include "bubbleManager.h"
#include "gameData.h"
#include "input.h"
#include "raylib.h"
#include "raymath.h"
#include "score.h"
#include <memory>
#include <raymob.h>


int GetPercent(int a, int total) { return (a / total) * 100; }

int main()
{
    Game game;
    game.Init();
    game.Run();
    game.Shutdown();
}

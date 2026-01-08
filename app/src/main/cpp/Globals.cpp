#include "Globals.h"

int Globals::screenWidth  = 0;
int Globals::screenHeight = 0;

void Globals::StartGame()
{
    SceneManager::Get().ActivateScene("GameplayScene");
    SceneManager::Get().DeactivateScene("HomeScene");

    LOGI("Gameplay scene activated");
}

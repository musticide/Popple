#include "Globals.h"

int Globals::screenWidth   = 0;
int Globals::screenHeight  = 0;

const int Globals::baseScreenWidth = 1080, Globals::baseScreenHeight = 2340;

void Globals::StartGame() {
    // SceneManager::Get().ActivateScene(SceneType::GAMEPLAY);
    // SceneManager::Get().DeactivateScene(SceneType::HOME);

    LOGI("Gameplay scene activated");
}

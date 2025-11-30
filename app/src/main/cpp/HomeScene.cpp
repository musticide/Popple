#include "HomeScene.h"
#include "Globals.h"
#include <memory>

HomeScene::HomeScene()
    : Scene("HomeScene")
{
    // playButton = std::make_unique<Button>("textures/start_button.png", WHITE, (Vector2) { 0, 0 });
    playButton = CreateEntity<Button>("textures/start_button.png", WHITE, (Vector2) { 0, 0 });
    playButton->AddOnClickListener(Globals::StartGame);
    playButton->SetActive(true);
}

HomeScene::~HomeScene() { }

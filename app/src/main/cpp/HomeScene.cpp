#include "HomeScene.h"
#include "Globals.h"
#include <memory>

#ifdef DEBUG

void StartEditor()
{
    SceneManager::Get().ActivateScene("EditorScene");
    SceneManager::Get().DeactivateScene("HomeScene");
    LOGI("Editor scene activated");
}

#endif // DEBUG

HomeScene::HomeScene()
    : Scene("HomeScene")
{
    // playButton = std::make_unique<Button>("textures/start_button.png", WHITE, (Vector2) { 0, 0 });
    playButton = CreateEntity<Button>(true, "textures/start_button.png", WHITE, (Vector2) { 0, 0 });
    playButton->AddOnClickListener(Globals::StartGame);
    touchFeedback = CreateEntity<TouchFeedback>(true);

#ifdef DEBUG
    editorButton = CreateEntity<Button>(true, "textures/EditorButton.png", WHITE, (Vector2) { 0, 256 });
    editorButton->AddOnClickListener(StartEditor);
#endif // DEBUG
}

HomeScene::~HomeScene() { }

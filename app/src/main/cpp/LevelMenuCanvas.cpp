#include "LevelMenuCanvas.h"
#include "GameManager.h"
#include "GameplayScene.h"
#include "LevelConfig.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "Scene.h"
#include "SceneManager.h"
#include "raylib.h"
#include "uiButton.h"
#include "uiCanvas.h"

LevelMenuCanvas::LevelMenuCanvas(Scene* parentScene)
: ui::Canvas(parentScene) {
    bgImage = CreateElement<ui::Image>(
        true, "textures/MainMenuBG.png", Rectangle{ 0, 0, 1080, 2340 }, ui::STRETCH_W | ui::STRETCH_H);

    Vector2 spacing = { 62.f, 28.f };
    for (size_t i = 0; i < levelBtns.size(); i++) {
        size_t levelNumber = i + 1; // offset by the level group
        Rectangle rect  = Rectangle{ 88, 316, 260, 260 };
        rect.x += (i % 3) * (rect.width + spacing.x);
        rect.y += std::floor(i / 3.f) * (rect.height + spacing.y);

        levelBtns[i] = CreateElement<LevelButton>(true, "textures/LevelsMenuAtlas.png", rect, ui::FIXED_W | ui::FIXED_H);

        levelBtns[i]->onClick.connect([this, i, &levelNumber]() {
            m_LevelConfig = (LevelConfig){ .level = levelNumber,
                .startSpawnInterval               = 1.2f,
                .minSpawnInterval                 = 0.7f - ((float)i * 0.01f),
                .spawnDecrementAmount             = 0.005f,
                .powerUpSpawnChance               = 30,
                .availablePowerUps                = { ElementType::ELECTRO } };
        });
        levelBtns[i]->SetLevelNumber(levelNumber);
        levelBtns[i]->SetClickable(levelNumber - 1 <= PlayerProfile.highestLevelCleared.value);
        levelBtns[i]->SetScale((levelNumber == PlayerProfile.highestLevelCleared.value + 1) ? 1.3f : 1.0f);
    }
    //TODO: Assign a default config

            // m_LevelConfig = (LevelConfig){ .level = (size_t)PlayerProfile.highestLevelCleared.value + 1,
            //     .startSpawnInterval               = 1.2f,
            //     .minSpawnInterval                 = 0.7f - (((float)PlayerProfile.highestLevelCleared.value + 1) * 0.01f),
            //     .spawnDecrementAmount             = 0.005f,
            //     .powerUpSpawnChance               = 30,
            //     .availablePowerUps                = { ElementType::ELECTRO } };

    playBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 191, 1732, 697, 350 }, ui::STRETCH_W | ui::FIXED_H);
    playBtn->drawRect = { 192, 270, 697, 350 };
    playBtn->onClick.connect([this]() {
        // Change scenemanager to activate scene by reference
        // Or register scene will create a scene inside the scenemanager
        // always only 1 scene of a type will exist
        SceneManager::Get().RegisterScene<GameplayScene>(false, m_LevelConfig);
        SceneManager::Get().ActivateScene(SceneType::GAMEPLAY);
        SceneManager::Get().DeactivateScene(SceneType::HOME);
    });

    backBtn = CreateElement<ui::Button>(
        true, "textures/LevelsMenuAtlas.png", Rectangle{ 53, 56, 128, 128 }, ui::FIXED_W | ui::FIXED_H);
    backBtn->SetAnchor({0.f, 0.f});
    backBtn->drawRect = { 740, 5, 128, 128 };
}

LevelMenuCanvas::~LevelMenuCanvas() {
}
void LevelMenuCanvas::Start() {
    ui::Canvas::Start();
    for (size_t i = 0; i < levelBtns.size(); i++) {

        levelBtns[i]->SetLevelNumber(i + 1);
    }
}

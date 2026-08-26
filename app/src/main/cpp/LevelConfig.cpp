#include "LevelConfig.h"
#include "GameManager.h"

LevelConfig GetLevelConfig(int levelNumber) {
    return (LevelConfig){ .isCleared = levelNumber <= PlayerProfile.highestLevelCleared.value,
        .levelNumber                 = levelNumber,
        .minScore                    = 160,
        .startSpawnInterval          = 1.0f,
        .minSpawnInterval            = 0.5f - ((float)(levelNumber - 1) * 0.01f),
        .spawnDecrementAmount        = 0.05f,
        .powerUpSpawnChance          = 30,
        .availablePowerUps           = { ElementType::ELECTRO } };
}

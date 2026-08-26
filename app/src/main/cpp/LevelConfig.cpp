#include "LevelConfig.h"
#include "ElementType.h"
#include "GameManager.h"
#include "RemoteConfig.h"
#include <cmath>

LevelParams GetLevelParams(int levelNumber) {
    int levelGroup                                     = std::floor((float)levelNumber / MAX_LEVEL_GROUPS);
    RemoteConfig::LevelGroupConfig& currentGroupConfig = RemoteConfig.levelGroupConfig[levelGroup];
    int levelNumberInGroup = levelNumber - ((levelGroup - 1) * MAX_LEVELS_IN_GROUP);

    LevelParams result;

    result.isCleared            = levelNumber <= PlayerProfile.highestLevelCleared.value;
    result.levelNumber          = levelNumber;
    result.minScore             = 160;
    result.startSpawnInterval   = currentGroupConfig.startSpawnInterval;
    result.minSpawnInterval     = currentGroupConfig.minSpawnInterval - (levelNumberInGroup * 0.01f);
    result.spawnDecrementAmount = 0.05f;
    result.powerUpSpawnChance   = currentGroupConfig.powerUpSpawnChance;

    for (size_t i = 0; i < (int)ElementType::COUNT; i++) {
        if (currentGroupConfig.availablePowerUps[i]) {
            result.availablePowerUps.push_back((ElementType)i);
        }
    }

    return result;
}

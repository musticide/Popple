#include "LevelConfig.h"
#include "ElementType.h"
#include "GameManager.h"
#include "RemoteConfig.h"
#include <cmath>

LevelParams GetLevelParams(int levelNumber) {
    LevelParams result;
    if (levelNumber > 0) {
        // (levelNumber - 1) shifts 1-120 to 0-119. 
        // Dividing by 12 gives a clean 0-indexed group (0 to 9)
        int levelGroupIndex = (levelNumber - 1) / MAX_LEVELS_IN_GROUP;
        
        RemoteConfig::LevelGroupConfig& currentGroupConfig = RemoteConfig.levelGroupConfig[levelGroupIndex];
        
        // Modulo 12 gives 0-11, adding 1 makes it 1-12 for your calculations
        int levelNumberInGroup = ((levelNumber - 1) % MAX_LEVELS_IN_GROUP) + 1;

        result.isCleared            = levelNumber <= PlayerProfile.highestLevelCleared.value;
        result.levelNumber          = levelNumber;
        result.minScore             = 160;
        result.startSpawnInterval   = currentGroupConfig.startSpawnInterval;
        
        // levelNumberInGroup will be 1 through 12
        result.minSpawnInterval     = currentGroupConfig.minSpawnInterval - (levelNumberInGroup * 0.01f);
        result.spawnDecrementAmount = 0.05f;
        result.powerUpSpawnChance   = currentGroupConfig.powerUpSpawnChance;

        for (size_t i = 0; i < (int)ElementType::COUNT; i++) {
            if (currentGroupConfig.availablePowerUps[i]) {
                result.availablePowerUps.push_back((ElementType)i);
            }
        }

        result.endlessMode = false;
    } else {
        int levelGroupIndex = (levelNumber - 1) / MAX_LEVELS_IN_GROUP;
        
        RemoteConfig::LevelGroupConfig& currentGroupConfig = RemoteConfig.levelGroupConfig[levelGroupIndex];
        result.isCleared                                   = true;
        result.levelNumber                                 = -1;
        result.minScore                                    = 0;
        result.startSpawnInterval                          = 1.2f;
        result.minSpawnInterval                            = 0.5f;
        result.spawnDecrementAmount                        = 0.05f;
        result.powerUpSpawnChance                          = 30;

        for (size_t i = 0; i < (int)ElementType::COUNT; i++) {
            if (currentGroupConfig.availablePowerUps[i]) {
                result.availablePowerUps.push_back((ElementType)i);
            }
        }
        result.endlessMode = true;
    }

    return result;
}

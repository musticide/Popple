#pragma once
#include "GameManager.h"
#include <array>

#define MAX_LEVEL_GROUPS 10
#define MAX_LEVELS_IN_GROUP 12

inline struct RemoteConfig {
    struct LevelGroupConfig {
        float startSpawnInterval;
        float minSpawnInterval;
        int powerUpSpawnChance;
        std::array<bool, (int)ElementType::COUNT> availablePowerUps;
    } levelGroupConfig[MAX_LEVEL_GROUPS];
} RemoteConfig;

void FetchRemoteConfig();

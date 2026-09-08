#pragma once
#include "ElementType.h"
#include "PlayerProfile.h"
#include <vector>

#define MAX_ELEMENTS_PER_LEVEL 4

struct LevelParams {
    bool isCleared;
    int levelNumber;
    int minScore;
    float startSpawnInterval;
    float minSpawnInterval;
    float spawnDecrementAmount;
    int powerUpSpawnChance;
    bool endlessMode;
    std::vector<ElementType> availablePowerUps;
};

LevelParams GetLevelParams(int levelNumber);

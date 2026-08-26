#pragma once
#include "ElementType.h"
#include "PlayerProfile.h"
#include <vector>

struct LevelParams {
    bool isCleared;
    int levelNumber;
    int minScore;
    float startSpawnInterval;
    float minSpawnInterval;
    float spawnDecrementAmount;
    int powerUpSpawnChance;
    std::vector<ElementType> availablePowerUps;
};

LevelParams GetLevelParams(int levelNumber);

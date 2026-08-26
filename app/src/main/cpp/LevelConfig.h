#pragma once 
#include "PlayerProfile.h"
#include <vector>
enum class ElementType;

struct LevelConfig{
    bool isCleared;
    int levelNumber;
    int minScore;
    float startSpawnInterval;
    float minSpawnInterval;
    float spawnDecrementAmount;
    int powerUpSpawnChance;
    std::vector<ElementType> availablePowerUps;

};

LevelConfig GetLevelConfig(int levelNumber);

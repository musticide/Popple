#pragma once 
#include <vector>
enum class ElementType;

struct LevelConfig{
    bool isCleared;
    int level;
    float startSpawnInterval;
    float minSpawnInterval;
    float spawnDecrementAmount;
    int powerUpSpawnChance;
    std::vector<ElementType> availablePowerUps;
};

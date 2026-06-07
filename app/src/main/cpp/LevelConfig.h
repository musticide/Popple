#pragma once 
#include <vector>
enum class ElementType;

struct LevelConfig{
    size_t level;
    float startSpawnInterval;
    float minSpawnInterval;
    float spawnDecrementAmount;
    int powerUpSpawnChance;
    std::vector<ElementType> availablePowerUps;
};

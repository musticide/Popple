#include "functionLibrary.h"
#include <random>
#include <raymob.h>

bool RollPercentage(int percentage){
    return GetRandomValue(0, 99) < percentage;
}

float RandomRangeFloat(float min, float max){
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dist(min, max);
    return dist(gen);
}


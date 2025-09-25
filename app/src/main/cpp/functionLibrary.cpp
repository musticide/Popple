#include "functionLibrary.h"
#include "raylib.h"

bool RollPercentage(int percentage){
    return GetRandomValue(0, 99) < percentage;

}

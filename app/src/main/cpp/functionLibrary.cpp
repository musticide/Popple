#include "functionLibrary.h"
#include <raymob.h>

bool RollPercentage(int percentage){
    return GetRandomValue(0, 99) < percentage;

}

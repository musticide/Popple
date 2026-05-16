#include "utils.h"

void Utils::SetUniformValue(const Shader& shader, const char* name, const void* value, int uniformType)
{
    SetShaderValue(shader, GetShaderLocation(shader, name), value, uniformType);
}
void Utils::SetUniformValue(const Shader& shader, int uniformLocation, const void* value, int uniformType)
{
    SetShaderValue(shader, uniformLocation, value, uniformType);
}
float Utils::EaseInOutCubic(float x) {
    float y = -2.f * x + 2.f;
    return x < 0.5f ? 4 * x * x * x : 1.f - (y * y * y) / 2.f;
}

#include "utils.h"

void Utils::SetUniformValue(const Shader& shader, const char* name, const void* value, int uniformType)
{
    SetShaderValue(shader, GetShaderLocation(shader, name), value, uniformType);
}
void Utils::SetUniformValue(const Shader& shader, int uniformLocation, const void* value, int uniformType)
{
    SetShaderValue(shader, uniformLocation, value, uniformType);
}

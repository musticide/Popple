#pragma once
#include <raymob.h>

#define GET_SHADER(model, i) model->materials[i].shader
#define TRANSPARENT_COLOR Color{255, 255, 255, 0}

namespace Utils {
void SetUniformValue(const Shader& shader, const char* name, const void* value, int uniformType);
void SetUniformValue(const Shader& shader, int uniformLocation, const void* value, int uniformType);
float EaseInOutCubic(float x);
} // namespace Utils

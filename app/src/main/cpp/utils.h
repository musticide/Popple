#pragma once
#include "deps/raylib/raylib.h"
#include <raymob.h>

#define GET_SHADER(model, i) model->materials[i].shader

namespace Utils {
    void SetUniformValue(const Shader& shader, const char* name, const void* value, int uniformType);
    void SetUniformValue(const Shader& shader, int uniformLocation, const void* value, int uniformType);
}

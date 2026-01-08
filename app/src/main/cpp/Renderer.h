#pragma once

#include "DrawableEntity.h"
#include "Globals.h"
#include "Singleton.h"
#include "raylib.h"
#include <memory>
#include <unordered_map>
#include <vector>

class Renderer : public Singleton<Renderer> {
public:
    Renderer();
    ~Renderer();

    void DrawScenes() const;
    void DrawUI() const;

    void     AddShader(std::weak_ptr<Shader> shader);

    /* void SetStaticUniforms()
    {
        Vector2 resolution
            = (Vector2) { (float)Globals::screenWidth, (float)Globals::screenHeight };
        for (auto& wShader : m_Shaders) {
            if (auto shader = wShader.lock()) {
                SetShaderUniformIfValid(
                    shader->shader, shader->globals.resolution, &resolution, SHADER_UNIFORM_VEC2);
            }
        }
    }
    void SetTimeUniforms(float time)
    {
        for (auto& wShader : m_Shaders) {
            if (auto shader = wShader.lock()) {
                SetShaderUniformIfValid(
                    shader->shader, shader->globals.time, &time, SHADER_UNIFORM_FLOAT);
            }
        }
    } */

private:
    std::vector<std::weak_ptr<Shader>> m_Shaders;
};

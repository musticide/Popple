#pragma once

#include <cstdlib>
#include "Assets.h"
#include <cstring>
#include "Log.h"
#include "glm/ext/vector_float4.hpp"
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#include <GLES3/gl3.h>
// #include <GLES3/gl32.h>

struct ShaderProgramSource {
    char* VertexShaderSource;
    char* FragmentShaderSource;
    size_t vertexShaderLength;
    size_t fragmentShaderLength;

    void Free()
    {
        if (VertexShaderSource) {
            free(VertexShaderSource);
            VertexShaderSource = nullptr;
        }
        if (FragmentShaderSource) {
            free(FragmentShaderSource);
            FragmentShaderSource = nullptr;
        }
    }
};

class Shader {
public:
    Shader();
    Shader(const char* filepath);
    ~Shader();

    void Bind() const;
    void Unbind() const;
    void Delete() const;

    GLint GetUniformLocation(const char* name);

    //set uniforms
    void SetUniform(const char* name, glm::vec4 value);
    static ShaderProgramSource ParseShader(const char *filepath);
    static GLuint CompileShader(GLenum type, const char *source);
    static GLuint CreateShaderProgram(ShaderProgramSource source);

private:
    int m_RendererID;
    const char* m_Filepath;

};

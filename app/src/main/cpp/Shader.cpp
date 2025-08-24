#include "Shader.h"
#include "Assets.h"
#include "Log.h"

Shader::Shader(const char* filepath)
    : m_Filepath(filepath)
    , m_RendererID(0)
{
    // CompileShader(GLenum type, const char *source);
    ShaderProgramSource source = ParseShader(filepath);
    m_RendererID = CreateShaderProgram(source);
    if(m_RendererID == 0)
    {
        LOGE("Unable to create shader: %s", filepath);
    }
    source.Free();
}

Shader::Shader()
    : m_Filepath("")
    , m_RendererID(0)
{
}

Shader::~Shader() {}

void Shader::Delete() const { glDeleteProgram(m_RendererID); }

ShaderProgramSource Shader::ParseShader(const char* filepath)
{
    ShaderProgramSource result;
    result.VertexShaderSource = nullptr;
    result.FragmentShaderSource = nullptr;

    if (!g_assetManager) {
        LOGE("Failed to Load Shader: %s", filepath);
        LOGE("Asset manager not initialized");
        return result;
    }

    AAsset* asset = AAssetManager_open(g_assetManager, filepath, AASSET_MODE_BUFFER);
    if (!asset) {
        LOGE("Failed to Load Shader: %s", filepath);
        return result;
    }

    size_t length = AAsset_getLength(asset);
    char* buffer = (char*)malloc(length + 1);
    AAsset_read(asset, buffer, length);
    buffer[length] = '\0';
    AAsset_close(asset);

    char* vertStart = strstr(buffer, "#shader vertex");
    char* fragStart = strstr(buffer, "#shader fragment");

    if (!vertStart || !fragStart) {
        free(buffer);
        LOGE("Vertex or Fragment shader directive not found in file: %s", filepath);
        return result;
    }

    vertStart = strchr(vertStart, '\n');
    if (vertStart)
        vertStart++;

    fragStart = strchr(fragStart, '\n');
    if (fragStart)
        fragStart++;

    if (!vertStart || !fragStart) {
        free(buffer);
        LOGE("Vertex or Fragment shader not found in file: %s", filepath);
        return result;
    }

    size_t vertLength, fragLength;

    char* vertEnd = strstr(vertStart, "#shader");
    if (vertEnd) {
        vertLength = vertEnd - vertStart;
    } else {
        vertLength = strlen(vertStart);
    }

    char* fragEnd = strstr(fragStart, "#shader");
    if (fragEnd) {
        fragLength = fragEnd - fragStart;
    } else {
        fragLength = strlen(fragStart);
    }

    while (vertLength > 0
        && (vertStart[vertLength - 1] == '\n' || vertStart[vertLength - 1] == '\r'
            || vertStart[vertLength - 1] == ' ')) {
        vertLength--;
    }
    while (fragLength > 0
        && (fragStart[fragLength - 1] == '\n' || fragStart[fragLength - 1] == '\r'
            || fragStart[fragLength - 1] == ' ')) {
        fragLength--;
    }

    result.VertexShaderSource = (char*)malloc(vertLength + 1);
    std::strncpy(result.VertexShaderSource, vertStart, vertLength);
    result.VertexShaderSource[vertLength] = '\0';
    result.vertexShaderLength = vertLength;

    result.FragmentShaderSource = (char*)malloc(fragLength + 1);
    std::strncpy(result.FragmentShaderSource, fragStart, fragLength);
    result.FragmentShaderSource[fragLength] = '\0';
    result.fragmentShaderLength = fragLength;

    free(buffer);

    return result;
}

GLuint Shader::CompileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
        GLint infoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);

        if (infoLength > 1) {
            char* infoLog = new char[infoLength];
            glGetShaderInfoLog(shader, infoLength, nullptr, infoLog);
            LOGE("Shader compile Error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

GLuint Shader::CreateShaderProgram(ShaderProgramSource source)
{
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, source.VertexShaderSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, source.FragmentShaderSource);

    if (vertexShader == 0 || fragmentShader == 0)
        return 0;

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
        GLint infoLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1) {
            char* infoLog = new char[infoLength];
            glGetProgramInfoLog(program, infoLength, 0, infoLog);
            LOGE("Program linking error: %s", infoLog);
            delete[] infoLog;
        }
        glDeleteProgram(program);
        return 0;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

GLint Shader::GetUniformLocation(const char* name)
{
    GLint location = glGetAttribLocation(m_RendererID, name);
    if (location == -1) {
        LOGW("Failed to get %s attrib location", name);
    }
    return location;
}

void Shader::Bind() const { glUseProgram(m_RendererID); }

void Shader::Unbind() const { glUseProgram(0); }

void Shader::SetUniform(const char* name, glm::vec4 value)
{
    glUniform4f(GetUniformLocation(name), value.x, value.y, value.z, value.w);

}

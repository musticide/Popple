#include "ResourceManager.h"
#include "Log.h"
#include "raylib.h"
#include <memory>

ResourceManager::ResourceManager() { }

ResourceManager::~ResourceManager() { CleanUpImpl(); }
void ResourceManager::CleanUpImpl()
{
    for (auto& [path, tex] : m_Textures) {
        UnloadTexture(*tex);
    }
    m_Textures.clear();

    for (auto& [path, tex] : m_Cubemaps) {
        UnloadTexture(*tex);
    }
    m_Cubemaps.clear();

    for (auto& [path, model] : m_Models) {
        UnloadModel(*model);
    }
    m_Models.clear();

    for (auto& [path, shader] : m_Shaders) {
        UnloadShader(*shader);
    }
    m_Shaders.clear();
}

std::shared_ptr<Texture2D> ResourceManager::GetTextureImpl(const char* filepath)
{
    auto it = m_Textures.find(filepath);
    if (it == m_Textures.end()) {
        LOGI("Trying to load texture: %s", filepath);
        m_Textures[filepath] = std::make_shared<Texture2D>(LoadTexture(filepath));
    }
    return m_Textures[filepath];
}

std::shared_ptr<TextureCubemap> ResourceManager::GetCubemapImpl(const char* filepath)
{
    auto it = m_Cubemaps.find(filepath);
    if (it == m_Cubemaps.end()) {
        Image img = LoadImage(filepath);
        if (img.data == NULL) {
            LOGE("Could not load Image");
            return nullptr;
        }
        ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
        m_Cubemaps[filepath]
            = std::make_shared<TextureCubemap>(LoadTextureCubemap(img, CUBEMAP_LAYOUT_CROSS_FOUR_BY_THREE));
        UnloadImage(img);
    }
    return m_Cubemaps[filepath];
}

std::shared_ptr<Shader> ResourceManager::GetShaderImpl(std::string vsPath, int fsPath)
{
    ShaderKey key;
    key.vsPath = vsPath;
    key.fsPath = "";
    auto it = m_Shaders.find(key);
    if (it == m_Shaders.end()) {
        m_Shaders[key] = std::make_shared<Shader>(LoadShader(vsPath.c_str(), 0));
    }
    return m_Shaders[key];
}

std::shared_ptr<Shader> ResourceManager::GetShaderImpl(int vsPath, std::string fsPath)
{
    ShaderKey key;
    key.vsPath = "";
    key.fsPath = fsPath;
    auto it = m_Shaders.find(key);
    if (it == m_Shaders.end()) {
        m_Shaders[key] = std::make_shared<Shader>(LoadShader(0, fsPath.c_str()));
    }
    return m_Shaders[key];
}

std::shared_ptr<Shader> ResourceManager::GetShaderImpl(std::string vsPath, std::string fsPath)
{
    ShaderKey key;
    key.vsPath = vsPath;
    key.fsPath = fsPath;
    auto it = m_Shaders.find(key);
    if (it == m_Shaders.end()) {
        m_Shaders[key] = std::make_shared<Shader>(LoadShader(vsPath.c_str(), fsPath.c_str()));
    }
    return m_Shaders[key];
}

std::shared_ptr<Model> ResourceManager::GetModelImpl(const char* filepath)
{
    auto it = m_Models.find(filepath);
    if (it == m_Models.end()) {
        m_Models[filepath] = std::make_shared<Model>(LoadModel(filepath));
    }
    return m_Models[filepath];
}

#pragma once

#include "raylib.h"
#include <cstddef>
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

struct ShaderKey {
    std::string vsPath;
    std::string fsPath;

    bool operator==(const ShaderKey& other) const noexcept { return vsPath == other.vsPath && fsPath == other.fsPath; }
};

struct ShaderKeyHash {
    std::size_t operator()(const ShaderKey& k) const noexcept
    {
        return std::hash<std::string>()(k.vsPath) ^ (std::hash<std::string>()(k.fsPath) << 1);
    }
};

class ResourceManager {
private:
    std::unordered_map<const char*, std::shared_ptr<Model>> m_Models;
    std::unordered_map<const char*, std::shared_ptr<Texture2D>> m_Textures;
    std::unordered_map<const char*, std::shared_ptr<TextureCubemap>> m_Cubemaps;
    std::unordered_map<ShaderKey, std::shared_ptr<Shader>, ShaderKeyHash> m_Shaders;

    ResourceManager();

public:
    ResourceManager(ResourceManager&&) = delete;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(ResourceManager&&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    ~ResourceManager();

    static ResourceManager& Get()
    {
        static ResourceManager instance = ResourceManager();
        return instance;
    }

    static std::shared_ptr<Model> GetModel(const char* filepath) { return Get().GetModelImpl(filepath); }
    static std::shared_ptr<Shader> GetShader(std::string vsPath, std::string fsPath) { return Get().GetShaderImpl(vsPath, fsPath); }
    static std::shared_ptr<Shader> GetShader(int vsPath, std::string fsPath) { return Get().GetShader(vsPath, fsPath); }
    static std::shared_ptr<Shader> GetShader(std::string vsPath, int fsPath) { return Get().GetShader(vsPath, fsPath); }
    static std::shared_ptr<TextureCubemap> GetCubemap(const char* filepath) { return Get().GetCubemap(filepath); }
    static std::shared_ptr<Texture2D> GetTexture(const char* filepath) { return Get().GetTexture(filepath); }

private:
    std::shared_ptr<Model> GetModelImpl(const char* filepath);

    std::shared_ptr<Shader> GetShaderImpl(std::string vsPath, std::string fsPath);

    std::shared_ptr<Shader> GetShaderImpl(int vsPath, std::string fsPath);

    std::shared_ptr<Shader> GetShaderImpl(std::string vsPath, int fsPath);

    std::shared_ptr<TextureCubemap> GetCubemapImpl(const char* filepath);

    std::shared_ptr<Texture2D> GetTextureImpl(const char* filepath);

    void CleanUpImpl();
};

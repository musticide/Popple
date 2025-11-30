#pragma once

#include "Scene.h"
#include "Singleton.h"
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

class SceneManager : public Singleton<SceneManager> {
public:
    void AddScene(Scene* scene);

    void StartScenes();

    void UpdateScenes(float dT);

    void DrawScenes();

    void DrawUI();

    Scene* GetScene(const char* name);

    void ActivateScene(const char* name);
    void DeactivateScene(const char* name);

    template <typename T> std::unique_ptr<T> RegisterScene()
    {
        static_assert(std::is_base_of_v<Scene, T>, "T does not derive from Scene");

        auto scene = std::make_unique<T>();
        assert(scene.get() && "Failed to create Scene");
        m_Scenes.push_back(scene.get());
        return std::move(scene);
    }

    SceneManager();
    ~SceneManager();

private:
    std::vector<Scene*> m_Scenes;
    std::vector<Scene*> m_ScenesToActivate;
    std::vector<Scene*> m_ScenesToDeactivate;
};

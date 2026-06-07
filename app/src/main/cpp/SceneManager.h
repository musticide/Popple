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
    void StartScenes();

    void UpdateScenes(float dT);

    // void DrawScenes();

    // void DrawUI();

    Scene* GetScene(SceneType type);

    void ActivateScene(SceneType type);
    void DeactivateScene(SceneType type);

    template <typename T, typename... Args>
    void RegisterScene(bool active, Args&&... args) {
        static_assert(std::is_base_of_v<Scene, T>, "T does not derive from Scene");

        auto scene = std::make_unique<T>(std::forward<Args>(args)...);
        scene->SetActive(active);

        int replaceAtIndex = scenes.size();
        for (size_t i = 0; i < scenes.size(); i++) {
            if (scene->GetType() == scenes[i]->GetType()) {
                replaceAtIndex = i;
            }
        }

        if (replaceAtIndex < scenes.size()) {
            scenes[replaceAtIndex] = std::move(scene);
        } else {
            scenes.push_back(std::move(scene));
        }
    }

    std::vector<std::unique_ptr<Scene>> scenes;

    SceneManager();
    ~SceneManager();

  private:
    // std::vector<Scene*> m_Scenes;
    std::vector<Scene*> m_ScenesToActivate;
    std::vector<Scene*> m_ScenesToDeactivate;
};

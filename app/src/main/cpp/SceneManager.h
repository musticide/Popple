#pragma once

#include "Log.h"
#include "Scene.h"
#include "Singleton.h"
#include <cassert>
#include <functional>
#include <memory>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

class SceneManager : public Singleton<SceneManager> {
  public:
    void LoadScenes();

    void StartScenes();

    void UpdateScenes(float dT);

    // void DrawScenes();

    // void DrawUI();

    Scene* GetScene(SceneType type);

    void ActivateScene(SceneType type);
    void DeactivateScene(SceneType type);
    void DestroyScene(SceneType type);
    int GetSceneIndex(SceneType type);

    template <typename T, typename... Args>
    void RegisterScene(SceneType type, bool active, Args&&... args) {
        static_assert(std::is_base_of_v<Scene, T>, "T does not derive from Scene");

        auto argsTuple  = std::make_tuple(std::forward<Args>(args)...);
        auto registerOp = [this, type, active, argsTuple = std::move(argsTuple)]() {
            RegisterSceneFromTuple<T>(type, active, std::move(argsTuple), std::index_sequence_for<Args...>{});
        };

        if (m_IsUpdatingScenes) {
            // Defer scene registration while iterating scenes to avoid mutating mid-update
            m_PendingOperations.push_back(std::move(registerOp));
        } else {
            registerOp();
        }
    }

    std::vector<std::unique_ptr<Scene>> scenes;
    float loadingProgress = 0.f;

    SceneManager();
    ~SceneManager();

  private:
    template <typename T, typename Tuple, size_t... I>
    void RegisterSceneFromTuple(SceneType type, bool active, Tuple&& argsTuple, std::index_sequence<I...>) {
        RegisterSceneInternal<T>(type, active, std::get<I>(std::forward<Tuple>(argsTuple))...);
    }

    template <typename T, typename... Args>
    void RegisterSceneInternal(SceneType type, bool active, Args&&... args) {
        static_assert(std::is_base_of_v<Scene, T>, "T does not derive from Scene");

        size_t target_index = scenes.size();
        for (size_t i = 0; i < scenes.size(); i++) {
            if (scenes[i] && scenes[i]->GetType() == type) {
                scenes[i].reset();
                target_index = i;
                break;
            }
        }

        auto scene = std::make_unique<T>(std::forward<Args>(args)...);
        scene->SetActive(active);
        LOGI("target index: %d, size %d", target_index, scenes.size());

        if (scene->showLoadingScreen) {
            ActivateScene(SceneType::LOADING);
        }

        LOGI("target index: %d, size %d", target_index, scenes.size());
        if (target_index < scenes.size())
            scenes[target_index] = std::move(scene);
        else
            scenes.push_back(std::move(scene));


        LOGI("target index: %d, size %d", target_index, scenes.size());
    }

    void ActivateSceneInternal(SceneType type) {
        m_ScenesToActivate.push_back(type);
    }
    void DeactivateSceneInternal(SceneType type) {
        m_ScenesToDeactivate.push_back(type);
    }
    void DestroySceneInternal(SceneType type) {
        m_ScenesToDestroy.push_back(GetSceneIndex(type));
    }

    // std::vector<Scene*> m_Scenes;
    std::vector<SceneType> m_ScenesToActivate;
    std::vector<SceneType> m_ScenesToDeactivate;
    std::vector<int> m_ScenesToDestroy;

    /// Operations queued while UpdateScenes is running; flushed at next StartScenes
    bool m_IsUpdatingScenes = false;
    std::vector<std::function<void()>> m_PendingOperations;

};

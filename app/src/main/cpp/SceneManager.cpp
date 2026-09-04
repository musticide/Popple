#include "SceneManager.h"
#include "Log.h"
#include "Scene.h"
#include <utility>

SceneManager::SceneManager() {
    LOGI("Scene Manager Created");
}

SceneManager::~SceneManager() {
}

//will work only if at a time a single scene is being loaded
void SceneManager::LoadScenes() {
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr) {
            if (scene->IsActive() && !scene->isLoaded) {
                scene->Load();
                loadingProgress = scene->GetProgress();
            }
        }
    }
}

void SceneManager::StartScenes() {
    // Flush operations deferred from the previous UpdateScenes pass first,
    // so registrations land before activation/deactivation is resolved.
    for (auto& op : m_PendingOperations) {
        if (op) op();
    }
    m_PendingOperations.clear();

    for (size_t i = 0; i < m_ScenesToDestroy.size(); i++) {
        int index = m_ScenesToDestroy[i];
        if (index >= 0 && static_cast<size_t>(index) < scenes.size()) {
            scenes[index].reset();
        } else {
            LOGE("Invalid scene destroy index: %d", index);
        }
    }
    m_ScenesToDestroy.clear();

    for (auto& type : m_ScenesToActivate) {
        Scene* scene = GetScene(type);
        if (scene != nullptr)
            scene->SetActive(true);
        else
            LOGE("Could not activate scene: scene not found");
    }
    m_ScenesToActivate.clear();

    for (auto& type : m_ScenesToDeactivate) {
        Scene* scene = GetScene(type);
        if (scene != nullptr)
            scene->SetActive(false);
        else
            LOGE("Could not deactivate scene: scene not found");
    }
    m_ScenesToDeactivate.clear();

    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr) {
            if (scene->IsActive() && scene->isLoaded) {
                // LOGV("Started Scene: %s", scene->GetName());
                scene->Start();
            }
        }
    }
}

void SceneManager::UpdateScenes(float dT) {
    m_IsUpdatingScenes = true;
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr)
            if (scene->IsActive() && scene->isLoaded) {
                // LOGV("Updating Scene: %s", scene->GetName());
                scene->Update(dT);
            }
    }
    m_IsUpdatingScenes = false;
}

int SceneManager::GetSceneIndex(SceneType type) {
    int result = -1;
    for (size_t i = 0; i < scenes.size(); i++) {
        Scene* scene = scenes[i].get();
        if (scene != nullptr) {
            if (scene->GetType() == type) result = i;
        } else {
            LOGE("Could not find scene");
        }
    }
    return result;
}

/// Can return NULLPTR
Scene* SceneManager::GetScene(SceneType type) {
    Scene* result = nullptr;
    for (size_t i = 0; i < scenes.size(); i++) {
        Scene* scene = scenes[i].get();
        if (scene != nullptr) {
            if (scene->GetType() == type) result = scene;
        } else {
            LOGE("Could not find scene");
        }
    }
    return result;
}
// void SceneManager::DeactivateScene(const char* name) { m_ScenesToDeactivate.push_back(GetScene(name)); }

// void SceneManager::ActivateScene(const char* name) { m_ScenesToActivate.push_back(GetScene(name)); }
void SceneManager::ActivateScene(SceneType type) {
    if (m_IsUpdatingScenes)
        m_PendingOperations.push_back([this, type]() { ActivateSceneInternal(type); });
    else
        ActivateSceneInternal(type);
}

void SceneManager::DeactivateScene(SceneType type) {
    if (m_IsUpdatingScenes)
        m_PendingOperations.push_back([this, type]() { DeactivateSceneInternal(type); });
    else
        DeactivateSceneInternal(type);
}

void SceneManager::DestroyScene(SceneType type) {
    if (m_IsUpdatingScenes)
        m_PendingOperations.push_back([this, type]() { DestroySceneInternal(type); });
    else
        DestroySceneInternal(type);
}

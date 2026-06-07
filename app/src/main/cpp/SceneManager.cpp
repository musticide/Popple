#include "SceneManager.h"
#include "Log.h"
#include "Scene.h"
#include <utility>

SceneManager::SceneManager() { LOGI("Scene Manager Created"); }

SceneManager::~SceneManager() { }

void SceneManager::StartScenes()
{
    for (auto& scene : m_ScenesToActivate) {
        if (scene != nullptr)
            scene->SetActive(true);
    }
    m_ScenesToActivate.clear();

    for (auto& scene : m_ScenesToDeactivate) {
        if (scene != nullptr)
            scene->SetActive(false);
    }
    m_ScenesToDeactivate.clear();

    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr) {
            if (scene->IsActive()) {
                // LOGV("Started Scene: %s", scene->GetName());
                scene->Start();
            }
        }
    }
}

void SceneManager::UpdateScenes(float dT)
{
    for (size_t i = 0; i < SceneManager::Get().scenes.size(); i++) {
        Scene* scene = SceneManager::Get().scenes[i].get();
        if (scene != nullptr)
            if (scene->IsActive()){
                // LOGV("Updating Scene: %s", scene->GetName());
                scene->Update(dT);
            }
    }
}

Scene* SceneManager::GetScene(SceneType type)
{
    Scene* result;
    for (size_t i = 0; i < scenes.size(); i++) {
        Scene* scene = scenes[i].get();
        if (scene != nullptr) {
            if (scene->GetType() == type)
                result = scene;
        } else {
            // LOGE("Could not find scene with name: %s", name);
        }
    }
    return result;
}
// void SceneManager::DeactivateScene(const char* name) { m_ScenesToDeactivate.push_back(GetScene(name)); }

// void SceneManager::ActivateScene(const char* name) { m_ScenesToActivate.push_back(GetScene(name)); }
void SceneManager::ActivateScene(SceneType type) {
    m_ScenesToActivate.push_back(GetScene(type));
}

void SceneManager::DeactivateScene(SceneType type) {
    m_ScenesToDeactivate.push_back(GetScene(type));
}


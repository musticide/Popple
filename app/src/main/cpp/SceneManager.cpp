#include "SceneManager.h"
#include "Log.h"
#include "Scene.h"
#include <utility>

SceneManager::SceneManager() { LOGI("Scene Manager Created"); }

SceneManager::~SceneManager() { }

void SceneManager::AddScene(Scene* scene)
{
    if (scene == nullptr) {
        LOGE("Could not add scene");
        return;
    }
    m_Scenes.push_back(scene);
}

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

    for (auto& scene : m_Scenes) {
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
    for (auto& scene : m_Scenes) {
        if (scene != nullptr)
            if (scene->IsActive()){
                // LOGV("Updating Scene: %s", scene->GetName());
                scene->Update(dT);
            }
    }
}

void SceneManager::DrawScenes()
{
    for (auto& scene : m_Scenes) {
        if (scene != nullptr)
            if (scene->IsActive())
                scene->Draw();
    }
}

void SceneManager::DrawUI()
{
    for (auto& scene : m_Scenes) {
        if (scene != nullptr)
            if (scene->IsActive())
                scene->DrawUI();
    }
}

Scene* SceneManager::GetScene(const char* name)
{
    Scene* result;
    for (auto& scene : m_Scenes) {
        if (scene != nullptr) {
            if (scene->GetName() == name)
                result = scene;
        } else {
            LOGE("Could not find scene with name: %s", name);
        }
    }
    return result;
}
void SceneManager::DeactivateScene(const char* name) { m_ScenesToDeactivate.push_back(GetScene(name)); }

void SceneManager::ActivateScene(const char* name) { m_ScenesToActivate.push_back(GetScene(name)); }

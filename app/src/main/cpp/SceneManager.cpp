#include "SceneManager.h"
#include "Log.h"
#include <utility>

SceneManager::SceneManager() {
    LOGI("Scene Manager Created");
}

SceneManager::~SceneManager() {
}


void SceneManager::AddScene(Scene* scene)
{
    if (scene == nullptr) {
        LOGE("Could not add scene");
        return;
    }
    m_Scenes.push_back(scene);
}

void SceneManager::LoadScene(Scene* scene)
{
    if (scene != nullptr)
        scene->Load();
}

void SceneManager::StartScenes()
{
    for (auto& scene : m_Scenes) {
        if (scene != nullptr) {
            if (!scene->HasLoaded())
                scene->Load();

            if (scene->IsActive())
                scene->Start();
        }
    }
}

void SceneManager::UpdateScenes(float dT)
{
    for (auto& scene : m_Scenes) {
        if (scene != nullptr)
            if (scene->IsActive())
                scene->Update(dT);
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


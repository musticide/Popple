#pragma once

#include "Scene.h"
#include "Singleton.h"
#include <vector>

class SceneManager : public Singleton<SceneManager> {
public:
    void AddScene(Scene* scene);

    void LoadScene(Scene* scene);

    void StartScenes();

    void UpdateScenes(float dT);

    void DrawScenes();

    void DrawUI();

    SceneManager();
    ~SceneManager();
private:
    std::vector<Scene*> m_Scenes;
};


// void LoadScene(std::unique_ptr<Scene> scene) { m_Scenes.push_back(std::move(scene)); }
// std::vector<std::unique_ptr<Scene>> m_Scenes;

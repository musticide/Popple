#include "Renderer.h"
#include "SceneManager.h"
#include "raylib.h"
#include <cstddef>

Renderer::Renderer() { }

Renderer::~Renderer() { }

void Renderer::DrawScenes() const
{
    //TODO: for each shader update globals
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawSky();
        //TODO: Draw sky only for the last scene for which sky is available
    }
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawOpaque();
    }
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawTransparent();
    }
}
void Renderer::DrawUI() const { 
    for (Scene* scene : SceneManager::Get().GetScenes()) {
        if (scene != nullptr && scene->IsActive())
            scene->DrawUI();
    }

}
void Renderer::AddShader(std::weak_ptr<Shader> shader) { 
    m_Shaders.push_back(shader); 
}


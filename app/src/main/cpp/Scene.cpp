#include "Scene.h"
#include "Log.h"
Scene::Scene(const char* name) 
:m_Name(name)
{
    LOGV("Scene created: %s", name);
}

Scene::~Scene() {
}
void Scene::AddEntity(std::unique_ptr<Entity> entity)
{
    LOGI("Adding to scene entities");

    LOGI("Adding to scene entities");
    Entity* e = entity.get();

    switch (e->GetRenderQueue()) {
    case RenderQueue::SKY:
        m_Buckets.sky.push_back(e);
        break;
    case RenderQueue::OPAQUE:
        m_Buckets.opaque.push_back(e);
        break;
    case RenderQueue::TRANSPARENT:
        m_Buckets.transparent.push_back(e);
        break;
    case RenderQueue::UI:
        m_Buckets.ui.push_back(e);
        break;
    }
    m_SceneEntities.push_back(std::move(entity));
}

void Scene::Load()
{
    if (!m_Loaded) {
        for (auto& e : m_SceneEntities) {
            if (e != nullptr)
            {
                e->LoadResources();
                LOGV("Scene Loaded: %s", m_Name);
            }
        }
        m_Loaded = true;
    }
}

void Scene::Start()
{
    if (!m_HasStarted) {
        for (auto& e : m_SceneEntities) {
            if (e != nullptr)
                if (e->IsActive())
                {
                    e->Start();
                    LOGV("Scene Started: %s", m_Name);
                }
        }
        m_HasStarted = true;
    }
}

void Scene::Draw()
{
    for (auto e : m_Buckets.sky)
        if (e != nullptr)
            if (e->IsActive())
                e->Draw();

    for (auto e : m_Buckets.opaque)
        if (e != nullptr)
            if (e->IsActive())
                e->Draw();

    for (auto e : m_Buckets.transparent)
        if (e != nullptr)
            if (e->IsActive())
                e->Draw();

}

void Scene::Update(float dT) { 
        for (auto& e : m_SceneEntities) {
            if (e != nullptr)
                if (e->IsActive())
                    e->Update(dT);
        }
}

void Scene::DrawUI() { 
    for (auto e : m_Buckets.ui)
        if (e != nullptr)
            if (e->IsActive())
                e->Draw();
}


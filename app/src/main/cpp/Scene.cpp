#include "Scene.h"
#include "Entity.h"
#include "Log.h"

Scene::Scene(const char* name)
    : m_Name(name)
{
    LOGV("Scene created: %s", name);
}

Scene::~Scene() { }

void Scene::Start()
{
    for (auto& e : m_SceneEntities) {
        if (e != nullptr && e->IsActive() && !e->hasStarted){
                e->Start();
                LOGV("Scene Started: %s", m_Name);
                e->hasStarted = true;
            }
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

void Scene::Update(float dT)
{
    for (auto& e : m_SceneEntities) {
        if (e != nullptr)
            if (e->IsActive() && e->hasStarted)
                e->Update(dT);
    }
}

void Scene::DrawUI()
{
    for (auto e : m_Buckets.ui)
        if (e != nullptr)
            if (e->IsActive() && e->hasStarted)
                e->Draw();
}

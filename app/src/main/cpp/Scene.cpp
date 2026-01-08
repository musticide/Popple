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
        if (e != nullptr && e->IsActive() && !e->hasStarted) {
            e->Start();
            LOGV("Scene Started: %s", m_Name);
            e->hasStarted = true;
        }
    }
}

void Scene::Update(float dT)
{
    for (auto& e : m_SceneEntities) {
        if (e != nullptr)
            if (e->IsActive() && e->hasStarted)
                e->Update(dT);
    }
}

void Scene::DrawSky() const
{
    for (auto d : m_Buckets.sky)
        if (d != nullptr)
            if (d->IsActive())
                d->Draw();
}

void Scene::DrawOpaque() const
{
    for (auto d : m_Buckets.opaque)
        if (d != nullptr)
            if (d->IsActive())
                d->Draw();
}

void Scene::DrawTransparent() const
{
    for (auto d : m_Buckets.transparent)
        if (d != nullptr)
            if (d->IsActive())
                d->Draw();
}
void Scene::DrawUI() const
{
    for (auto d : m_Buckets.ui)
        if (d != nullptr)
            if (d->IsActive() && d->hasStarted)
                d->Draw();
}

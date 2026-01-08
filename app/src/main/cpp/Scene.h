#pragma once

#include "DrawableEntity.h"
#include "Entity.h"
#include "Log.h"
#include "ParticleSystem.h"
#include <exception>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

enum class SceneType { HOME, GAMEPLAY };

class Scene {
public:
    Scene(const char* name);
    Scene(Scene&&) = default;
    Scene(const Scene&) = default;
    Scene& operator=(Scene&&) = default;
    Scene& operator=(const Scene&) = default;
    ~Scene();

    template <typename T, typename... Args> std::unique_ptr<T> CreateEntity(bool active, Args&&... args)
    {
        static_assert(std::is_base_of<Entity, T>::value, "T must derive from Entity");

        LOGV("%s is creating entity", m_Name);
        auto entity = std::make_unique<T>(std::forward<Args>(args)...);

        entity->parentScene = this;
        entity->SetActive(active);

        LOGV("Entity added to %s", m_Name);
        m_SceneEntities.push_back(entity.get());

        if (DrawableEntity* d = entity->asDrawable()) {
            LOGV("Entity is Drawable");
            switch (d->GetRenderQueue()) {
            case RenderQueue::SKY:
                m_Buckets.sky.push_back(d);
                break;
            case RenderQueue::OPAQUE:
                m_Buckets.opaque.push_back(d);
                break;
            case RenderQueue::TRANSPARENT:
                // if (dynamic_cast<ParticleSystem*>(d))
                //     LOGI("Particle System added");
                m_Buckets.transparent.push_back(d);
                break;
            case RenderQueue::UI:
                m_Buckets.ui.push_back(d);
                break;
            }
        }

        return std::move(entity);
    }

    const char* const GetName() const { return m_Name; }

    bool IsActive() const { return m_IsActive; }
    void SetActive(bool active)
    {
        m_IsActive = active;
        if (active) {
            LOGI("Scene activated: %s", m_Name);
            for (auto e : m_SceneEntities) {
                if (e != nullptr && e->IsActive()) {
                    e->OnEnable();
                }
            }

        } else {
            LOGI("Scene deactivated: %s", m_Name);
            for (auto e : m_SceneEntities) {
                if (e != nullptr && e->IsActive()) {
                    e->OnDisable();
                }
            }
        }
    }

    void Start();
    void Update(float dT);
    void DrawSky() const;
    void DrawOpaque() const;
    void DrawTransparent() const;
    void DrawUI() const;

protected:
    const char* m_Name;
    std::vector<Entity*> m_SceneEntities;

    struct RenderBuckets {
        std::vector<DrawableEntity*> sky;
        std::vector<DrawableEntity*> opaque;
        std::vector<DrawableEntity*> transparent;
        std::vector<DrawableEntity*> ui;
    } m_Buckets;

private:
    bool m_IsActive;
};
